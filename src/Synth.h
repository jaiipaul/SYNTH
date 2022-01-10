#ifndef _SYNTH_H_
#define _SYNTH_H_

#include "gui.h"
#include "portaudio.h"
#include "typeDef.h"
#include "Modules.h"

class Synth{
private:
    //Audio attributes
    unsigned long sampleRate;
    unsigned long bufferSize;

    //GUI Attributes
    Gui* __gui__;
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
    static void SynthOutput(Mixer* Mix, LadderFilter* Filt, Amplifier* Amp, 
                            std::vector<LFO*> _LFOs, 
                            std::vector<AR_envelop*> _ARs, 
                            std::vector<ADSR_envelop*> _ADSRs, 
                            PaData* Data);

    static int Callback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

    bool isRunning();
    void handleEvents();
    void update();
    void render();
    void clean();

};

#endif // _SYNTH_H_