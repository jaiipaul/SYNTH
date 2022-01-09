#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "gui.h"
#include "portaudio.h"
#include "typeDef.h"
#include "Modules.h"

class Synth : public Gui{
private:
    //Audio attributes
    unsigned long sampleRate;
    unsigned long bufferSize;

    //GUI Attributes
    const char* name;
    int width;
    int height;
    
public:
    //Oscillator*  Osc;
    Keyboard* __keyboard__;

    Synth();
    ~Synth();

    void initSynth(const char* _name, int _width, int _height, bool fullscreen,
                   unsigned long _sampleRate, unsigned long _bufferSize);

    void buildSynthModules();
    void buildSynthGui();
    static void SynthOutput(Mixer* Mix, LadderFilter* Filt, LFO* lfo, Amplifier* Amp, PaData* Data);

    static int Callback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

    void update();
    void clean();

};

#endif // _SYNTH_H_