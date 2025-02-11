#!/usr/bin/env python3

# pyre-unsafe

# Copyright (c) Facebook, Inc. and its affiliates.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

import tempfile
import numpy as np
import crypten
import crypten.communicator as comm
from crypten.mpc.ptype import ptype as Ptype
import torch
import torch.nn as nn
import torch.nn.functional as F
from crypten_utils.util import NoopContextManager
from torchvision import datasets, transforms
# from heart_disease_data import heart_disease_data
import pandas as pd
from crypten.common.util import torch_cat, torch_stack
# 统计通信量
from crypten.config import cfg
from crypten.mpc import MPCTensor

cfg.communicator.verbose = True


class LogisticRegressionModel(crypten.nn.Module):
    def __init__(self, n_features):
        super(LogisticRegressionModel, self).__init__()
        self.lr = crypten.nn.Linear(n_features, 1)

    def forward(self, x):
        out = self.lr(x).sigmoid()
        return out


def run_mpc_autograd_lr(
    context_manager=None,
    num_epochs=3,
    learning_rate=0.001,
    batch_size=100,
    print_freq=5,
    filenames=[]
):
    """
    Args:
        context_manager: used for setting proxy settings during download.
    """
    # Specify file locations to save each piece of data
    # filenames = {
    #     "p0": "../CFIX_output_file.cs0",
    #     "p1": "../CFIX_output_file.cs1"
    # }

    crypten.init()

    # read mpc shares from file
    rank = comm.get().get_rank()
    if rank == 0:
        data = pd.read_csv(filenames[0], header=None) 
    else:
        data = pd.read_csv(filenames[1], header=None) 
    print("data=", data)
    feature = data.iloc[:,0:-1]
    label = data.iloc[:,-1:]
    feature = MPCTensor.from_shares(torch.tensor(feature.values))
    label = MPCTensor.from_shares(torch.tensor(label.values), ptype=Ptype.binary)
    label = label.to(ptype=Ptype.arithmetic)
    n_features = feature.shape[1]
    # encrypt plaintext model
    model = LogisticRegressionModel(n_features).encrypt()

    # encrypted training
    train_encrypted(
        feature, label, model, num_epochs, learning_rate, batch_size, print_freq
    )

    comm.get().print_communication_stats()



def reduce_xor(x, dim=None):  # zhangqizhi.zqz
    if dim!=0:
        raise NotImplementedError("BinarySharedTensor reduce_xor not implemented")
    else:
        while x.size(0) > 1:
            extra = None
            if x.size(0) % 2 == 1:
                extra = x[0]
                x = x[1:]
            x0 = x[: (x.size(0) // 2)]
            x1 = x[(x.size(0) // 2) :]
            x = x0 ^ x1
            if extra is not None:
                x.share = torch_cat([x.share, extra.share.unsqueeze(0)])
        x = x.squeeze(0)
        return x



def run_mpc_autograd_lr_with_real_flag(
    context_manager=None,
    num_epochs=3,
    learning_rate=0.001,
    batch_size=100,
    print_freq=5,
    filenames=[]
):
    """
    Args:
        context_manager: used for setting proxy settings during download.
    """
    # Specify file locations to save each piece of data

    crypten.init()

    # read mpc shares from file
    rank = comm.get().get_rank()
    if rank == 0:
        data = pd.read_csv(filenames[0], header=None) 
    else:
        data = pd.read_csv(filenames[1], header=None) 
    # data = np.reshape(data.values, [-1, 1305, data.shape[1]])
    print("data=", data)
    data = np.reshape(data.values, [1305, data.shape[1]])
    feature = data[:, 0:-2]
    label = data[:, -2:-1]
    real_flag = data[:, -1:]
    
    feature = MPCTensor.from_shares(torch.tensor(feature), ptype=Ptype.binary)
    label = MPCTensor.from_shares(torch.tensor(label), ptype=Ptype.binary)
    real_flag = MPCTensor.from_shares(torch.tensor(real_flag), ptype=Ptype.binary)
    feature._tensor &= real_flag._tensor
    label._tensor &= real_flag._tensor
    # feature._tensor = reduce_xor(feature._tensor, dim=0) 
    # label._tensor = reduce_xor(label._tensor, dim=0) 
    
    
    feature = feature.to(ptype=Ptype.arithmetic)
    label = label.to(ptype=Ptype.arithmetic)
    n_features = feature.shape[1]
    # encrypt plaintext model
    model = LogisticRegressionModel(n_features).encrypt()

    # encrypted training
    train_encrypted(
        feature, label, model, num_epochs, learning_rate, batch_size, print_freq
    )

    comm.get().print_communication_stats()




def train_encrypted(
    x_encrypted,
    y_encrypted,
    encrypted_model,
    num_epochs,
    learning_rate,
    batch_size,
    print_freq,
):
    rank = comm.get().get_rank()
    loss = crypten.nn.BCELoss()
    num_samples = x_encrypted.size(0)

    for epoch in range(num_epochs):
        last_progress_logged = 0
        # only print from rank 0 to avoid duplicates for readability
        if rank == 0:
            print(f"Epoch {epoch} in progress:")

        for j in range(0, num_samples, batch_size):
            # define the start and end of the training mini-batch
            start, end = j, min(j + batch_size, num_samples)

            # switch on autograd for training examples
            x_train = x_encrypted[start:end]
            x_train.requires_grad = True
            # y_one_hot = label_eye[y_encrypted[start:end]]
            y_train = y_encrypted[start:end]
            y_train.requires_grad = True

            # perform forward pass:
            output = encrypted_model(x_train)
            loss_value = loss(output, y_train)

            # backprop
            encrypted_model.zero_grad()
            loss_value.backward()
            encrypted_model.update_parameters(learning_rate)

            # log progress
            if j + batch_size - last_progress_logged >= print_freq:
                last_progress_logged += print_freq
                print(f"Loss {loss_value.get_plain_text().item():.4f}")

        # compute accuracy every epoch
        pred = output.get_plain_text()
        correct = torch.abs(y_train.get_plain_text() - pred) < 0.5
        accuracy = correct.float().mean()
        loss_plaintext = loss_value.get_plain_text().item()
        print(
            f"Epoch {epoch} completed: "
            f"Loss {loss_plaintext:.4f} Accuracy {accuracy.item():.2f}"
        )
