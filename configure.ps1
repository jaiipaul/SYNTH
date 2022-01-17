if(!'build'){
    mkdir build 
}
cmake -DPNG_TESTS=OFF -DSUPPORT_JPG=OFF -DSUPPORT_WEBP=OFF -S . -B .\build


