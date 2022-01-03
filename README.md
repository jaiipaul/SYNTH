# SYNTH APP 
Synth App using SDL2 and PortAudio


## Installation for WINDOWS
1) Visual Studio 2019 should be installed
2) Compile PortAudio library :  

     ```Powershell 
     cd libs/portaudio/
     mkdir build && cd build && cmake ..
     cmake --build . --config Release --target ALL_BUILD -j 10 --
     cd ../../..
     ```

3) Then Compile sources using build powershell script :
    
    ```Powershell
    ./build.ps1
    ```

4) The application is in `bin/` folder, you can run from there.
   You can also use the run powershell script :

    ```Powershell
    ./run.ps1
    ```