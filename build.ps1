rm -r build
mkdir build 
cd build 
cmake ..
cmake --build . --config Release --target ALL_BUILD -j 10 --
cd ..
