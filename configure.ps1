if(!'build'){
    mkdir build 
}
cmake -DPNG_TESTS=OFF -S . -B .\build


