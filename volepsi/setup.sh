# export http_proxy=http://sys-proxy-rd-relay.byted.org:8118
# export https_proxy=http://sys-proxy-rd-relay.byted.org:8118
CMAKE_CXX_COMPILER=/usr/local/gcc/bin/g++
CMAKE_C_COMPILER=/usr/local/gcc/bin/gcc

python3 build.py --debug -DENABLE_SPAN_LITE=ON -DVOLE_PSI_ENABLE_BOOST=ON  -DVOLE_PSI_ENABLE_CPSI=ON -DVOLE_PSI_ENABLE_OPPRF=ON  -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}

