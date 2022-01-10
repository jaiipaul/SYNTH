#include <iostream>
#include <portaudio.h>
#include "Synth.h"

using namespace std;

Synth* __Synth__ = nullptr;

int main(int argc, char* argv[]){
    cout << "AUDIO APP !" << endl;
    __Synth__ = new Synth();
    
    __Synth__->initSynth("Synth", 1000, 600, false, 44100, 256);

    while(__Synth__->isRunning()){
        __Synth__->handleEvents();
        __Synth__->update();
        __Synth__->render();
    }
    
    __Synth__->clean();

    return 0;
}

