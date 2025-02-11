
file(GLOB_RECURSE SRCS *.cpp)

add_executable(cpsi_test  ${SRCS})

target_link_libraries(cpsi_test volePSI_Tests)


if(VOLE_PSI_NO_LTO)
    target_link_options(cpsi_test PUBLIC "-fno-lto")
endif()



if(MSVC)
    target_compile_options(cpsi_test PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/std:c++${VOLE_PSI_STD_VER}>)
    #target_compile_options(libOTe PRIVATE -openmp:experimental)
else()
    target_compile_options(cpsi_test PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++${VOLE_PSI_STD_VER}>)
endif()