cd extern/volePSI
python3 build.py --debug -DVOLE_PSI_ENABLE_BOOST=ON  -DVOLE_PSI_ENABLE_OPPRF=ON  -DCMAKE_CXX_STANDARD=20
cd ../..
cmake --no-warn-unused-cli  -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE  -DCMAKE_C_COMPILER:FILEPATH=/usr/local/gcc/bin/gcc -DCMAKE_CXX_COMPILER:FILEPATH=/usr/local/gcc/bin/g++  -DCMAKE_CXX_STANDARD=20 -B build .
cmake --build build
