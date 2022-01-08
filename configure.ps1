if(!'build'){
    mkdir build
}
cmake -DRTMIDI_BUILD_TESTING=OFF -DBUILD_SHARED_LIBS=OFF -S . -B .\build


