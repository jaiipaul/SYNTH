#include "Synth.h"
#include "Widgets.h"
#include "Oscillator.h"
#include "Amplifier.h"
#include "Envelops.h"
#include "Filter.h"
#include "keyboard.h"
#include <functional>   // std::bind

static AudioEngine* __audioEngine__ = new AudioEngine();
static Oscillator* __Osc__ = new Oscillator();
static Amplifier*  __Amp__ = new Amplifier();
static Filter* __Filter__  = new Filter();

void StartStream();
void StopStream();
void ListDevices();
void SetVolume(double coef);
void SetEnvLvl(double coef);
void SetAR_Attack(double coef);
void SetAR_Release(double coef);
void SetADSR_Attack(double coef);
void SetADSR_Decay(double coef);
void SetADSR_Sustain(double coef);
void SetADSR_Release(double coef);
void SwitchEnvType();
void SetCutoff(double coef);

Synth::Synth(){

}

Synth::~Synth(){
}

int Synth::Callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{    
    /* Cast data passed through stream to our structure. */
    PaData *callbackData = (PaData*)userData; 
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    for( i=0; i<framesPerBuffer; i++ )
    {   

        SynthOutput(__Osc__, __Filter__, __Amp__, callbackData);
        out[2*i]   = callbackData->left_phase;  /* left */
        out[2*i+1] = callbackData->right_phase; /* right */
    }
    return 0;
}

void Synth::initSynth(const char* _name, int _width, int _height, bool fullscreen,
                      unsigned long _sampleRate, unsigned long _bufferSize){
    sampleRate = _sampleRate; bufferSize = _bufferSize;         
    __audioEngine__->init(sampleRate, bufferSize);
    std::cout << Callback << std::endl;
    __audioEngine__->SetCallBackFunc(&Callback);
    std::cout << (__audioEngine__->callbackFunc) << std::endl;
    __audioEngine__->OpenDefaultStream();
    const PaStreamInfo* streamInfo = Pa_GetStreamInfo(__audioEngine__->stream);
    std::cout << streamInfo->sampleRate << std::endl;

    name = _name; width = _width; height = _height;
    init("Synth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen);




    buildSynthGui();
    buildSynthModules();

}


void Synth::buildSynthGui(){
    buttonFunc startStream  = &StartStream;
    buttonFunc stopStream   = &(StopStream);
    buttonFunc listDevices  = &(ListDevices);
    sliderFunc setVolume    = &(SetVolume);
    sliderFunc setEnvLvl    = &(SetEnvLvl);
    sliderFunc setAR_Attack    = &(SetAR_Attack);
    sliderFunc setAR_Release   = &(SetAR_Release);
    sliderFunc setADSR_Attack  = &(SetADSR_Attack);
    sliderFunc setADSR_Decay   = &(SetADSR_Decay);
    sliderFunc setADSR_Sustain = &(SetADSR_Sustain);
    sliderFunc setADSR_Release = &(SetADSR_Release);
    buttonFunc switchEnvType   = &(SwitchEnvType);
    sliderFunc setCutoff       = &(SetCutoff);

    Button* button1 = new Button(renderer, 10, 10, 64, 16, "../../assets/button.png");
    button1->setFunction(startStream);
    Button* button2 = new Button(renderer, 10, 36, 64, 16, "../../assets/button.png");
    button2->setFunction(stopStream);
    Button* button3 = new Button(renderer, 10, 62, 64, 16, "../../assets/button.png");
    button3->setFunction(listDevices);
    // AMP sliders
    Slider* slider1 = new Slider(renderer, 100, 200, 32, 8, 100, "../../assets/button.png");
    slider1->setFunction(setVolume);
    Slider* slider2 = new Slider(renderer, 150, 200, 32, 8, 100, "../../assets/button.png");
    slider2->setFunction(setEnvLvl);
    // AR Sliders
    Slider* slider3 = new Slider(renderer, 250, 200, 32, 8, 100, "../../assets/button.png");
    slider3->setFunction(setAR_Attack);
    Slider* slider4 = new Slider(renderer, 300, 200, 32, 8, 100, "../../assets/button.png");
    slider4->setFunction(setAR_Release);
    // ADSR Sliders
    Slider* slider5 = new Slider(renderer, 400, 200, 32, 8, 100, "../../assets/button.png");
    slider5->setFunction(setADSR_Attack);
    Slider* slider6 = new Slider(renderer, 450, 200, 32, 8, 100, "../../assets/button.png");
    slider6->setFunction(setADSR_Decay);
    Slider* slider7 = new Slider(renderer, 500, 200, 32, 8, 100, "../../assets/button.png");
    slider7->setFunction(setADSR_Sustain);
    Slider* slider8 = new Slider(renderer, 550, 200, 32, 8, 100, "../../assets/button.png");
    slider8->setFunction(setADSR_Release);
    // ENV selector
    Button* button4 = new Button(renderer, 250, 150, 64, 16, "../../assets/button.png");
    button4->setFunction(switchEnvType);
    //Filter 
    Slider* slider9 = new Slider(renderer, 550, 50, 32, 8, 100, "../../assets/button.png");
    slider9->setFunction(setCutoff);

    addWidget(button1);
    addWidget(button2);
    addWidget(button3);
    addWidget(slider1);
    addWidget(slider2);
    addWidget(slider3);
    addWidget(slider4);
    addWidget(slider5);
    addWidget(slider6);
    addWidget(slider7);
    addWidget(slider8);
    addWidget(button4);
    addWidget(slider9);
}

void Synth::buildSynthModules(){
    __keyboard__ = new Keyboard();
    __Osc__->init("sqr", sampleRate, __keyboard__);
    __Amp__->init(sampleRate, __keyboard__);
    __Filter__->init(sampleRate);
}

void Synth::SynthOutput(Oscillator* Osc, Filter* Filt, Amplifier* Amp, PaData* Data){
    Osc->generateWave(Data);
    Filt->genOutput(Data);
    Amp->genOutput(Data);
}

void Synth::update(){
    for(auto w : Widgets){
        w->Update();
    }
    __keyboard__->Update();
}   

void Synth::clean(){
    __audioEngine__->clean();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Synth cleaned!..." << std::endl;
}

// AUDIO ENGINE 
void StartStream(){
    std::cout << "synth start stream" << std::endl;
    __audioEngine__->StartStream();
}
void StopStream(){
    std::cout << "synth stop stream" << std::endl;
    __audioEngine__->StopStream();
}
void ListDevices(){
    __audioEngine__->ListDevices();
}

// AMPLIFIER
void SetVolume(double coef){
    __Amp__->setVolume(coef);
}
void SetEnvLvl(double coef){
    __Amp__->setEnvelopLevel(coef);
}

// AR AMP ENVELOP
void SetAR_Attack(double coef){
    __Amp__->AR->setAttack(coef);
}
void SetAR_Release(double coef){
    __Amp__->AR->setRelease(coef);
}

// ADSR AMP ENVELOP
void SetADSR_Attack(double coef){
    __Amp__->ADSR->setAttack(coef);
}
void SetADSR_Decay(double coef){
    __Amp__->ADSR->setDecay(coef);
}
void SetADSR_Sustain(double coef){
    __Amp__->ADSR->setSustain(coef);
}
void SetADSR_Release(double coef){
    __Amp__->ADSR->setRelease(coef);
}

void SwitchEnvType(){
    __Amp__->switchEnvelopType();
}

void SetCutoff(double coef){
    __Filter__->setCutoff(coef);
}