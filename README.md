# SYNTH APP 
Synth App using [SDL2](https://github.com/libsdl-org/SDL.git), [PortAudio](https://github.com/PortAudio/portaudio.git) and [LibreMidi](https://github.com/jcelerier/libremidi.git).

To build the project make sure you have CMake installed.

## On LINUX and macOS
```Bash
mkdir build && cd build 
cmake ..
make
```
The application is installed in `.\bin` folder.

## On Windows
With [Visual Studio 2019](https://visualstudio.microsoft.com/fr/vs/older-downloads/#visual-studio-2019-and-other-products) installed, or only Build Tools, you can run powershell scripts as follows:

1) Run `configure.ps1`, this will launch the cmake configuration.

2) Run `build.ps1` to build the project.
3) Run `run.ps1` to launch the application.


