if(!'build'){
    mkdir build
}
cmake -DBUILD_SHARED_LIB=OFF -S . -B .\build


