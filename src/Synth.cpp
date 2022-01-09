#include <vector>

#include "Synth.h"
#include "Widgets.h"
#include "Modules.h"
#include "SynthControls.h"

//////// SYNTH MODULES ////////
/* AUDIO ENGINE */
static AudioEngine* __audioEngine__ = new AudioEngine();
/* MIXER */
static Mixer* __Mix__ = new Mixer();
/* OSCILLATORS */
static std::vector<Oscillator*> Oscillators;
static Oscillator* __Osc1__ = new Oscillator();
static Oscillator* __Osc2__ = new Oscillator();

/* VCA */
static Amplifier*  __Amp__ = new Amplifier();
/* VCF */
static LadderFilter* __Filter__ = new LadderFilter();
/* LFOs */
static LFO* __LFO__ = new LFO();



//////// SYNTH CLASS DEFINITION /////////
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

        SynthOutput(__Mix__, __Filter__, __LFO__, __Amp__, callbackData);
        out[2*i]   = 0.5*(callbackData->left_phase < 1.f)  ? callbackData->left_phase  : 1.f;  /* left */
        out[2*i+1] = 0.5*(callbackData->right_phase < 1.f) ? callbackData->right_phase : 1.f; /* right */
    }
    return 0;
}

void Synth::initSynth(const char* _name, int _width, int _height, bool fullscreen,
                      unsigned long _sampleRate, unsigned long _bufferSize){
    /* AUDIO ENGINE INITIALISATION */
    sampleRate = _sampleRate; bufferSize = _bufferSize;         
    __audioEngine__->init(sampleRate, bufferSize);
    __audioEngine__->SetCallBackFunc(&Callback);
    __audioEngine__->OpenDefaultStream();

    /* SYNTH INITIALISATION */
    name = _name; width = _width; height = _height;
    init("Synth", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen);

    buildSynthGui();
    std::cout << "Built Gui..." << std::endl;
    buildSynthModules();
    std::cout << "Built Modules..." << std::endl;
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

/////// SYNTH MODULES INITILISATION ////////
void Synth::buildSynthModules(){
    __keyboard__ = new Keyboard();
    __keyboard__->init(true);

    __Osc1__->init(2, 3, sampleRate, __keyboard__);
    __Osc2__->init(2, 3, sampleRate, __keyboard__);
    Oscillators.push_back(__Osc1__);
    Oscillators.push_back(__Osc2__);
    __Mix__->init(2);
    __Amp__->init(sampleRate, __keyboard__);
    __Filter__->init(96000.f);
    __LFO__->init(3, sampleRate, __keyboard__);
    __Osc1__->setLFO(__LFO__);
    __Osc2__->setLFO(__LFO__);
}

/////// SYNTH SOUND PATH ///////
void Synth::SynthOutput(Mixer* Mix, LadderFilter* Filt, LFO* Lfo, Amplifier* Amp, PaData* Data){
    __LFO__->generateWave();
    __Mix__->generateOutput(Data, Oscillators);
    __Filter__->genOutput(Data);
    __Amp__->genOutput(Data);
}

////////  SYNTH GUI /////////
void Synth::buildSynthGui(){
    buttonFunc startStream     = &(StartStream);
    buttonFunc stopStream      = &(StopStream);
    buttonFunc listDevices     = &(ListDevices);
    sliderFunc setPulseWidth   = &(SetPulseWidth);
    sliderFunc setVolume       = &(SetVolume);
    sliderFunc setEnvLvl       = &(SetEnvLvl);
    sliderFunc setAR_Attack    = &(SetAR_Attack);
    sliderFunc setAR_Release   = &(SetAR_Release);
    sliderFunc setADSR_Attack  = &(SetADSR_Attack);
    sliderFunc setADSR_Decay   = &(SetADSR_Decay);
    sliderFunc setADSR_Sustain = &(SetADSR_Sustain);
    sliderFunc setADSR_Release = &(SetADSR_Release);
    buttonFunc switchEnvType   = &(SwitchEnvType);
    sliderFunc setCutoff       = &(SetCutoff);
    sliderFunc setResonance    = &(SetResonance);
    buttonFunc switchWaveForm  = &(SwitchWaveForm);
    sliderFunc setFreqMod      = &(SetFreqMod);
    sliderFunc setPwMod        = &(SetPwMod);
    sliderFunc setLfoRate      = &(SetLfoRate);
    sliderFunc setDetune       = &(SetDetune);

    const char* buttonTextPath = "../../../assets/button.png";
    const char* buttonPurple = "../../../assets/button_purple.png";

    Button* button1 = new Button(renderer, 10, 10, 64, 16, buttonTextPath);
    button1->setFunction(startStream);
    Button* button2 = new Button(renderer, 10, 36, 64, 16, buttonTextPath);
    button2->setFunction(stopStream);
    Button* button3 = new Button(renderer, 10, 62, 64, 16, buttonTextPath);
    button3->setFunction(listDevices);
    // AMP sliders
    Slider* slider1 = new Slider(renderer, 100, 200, 32, 8, 100, buttonTextPath);
    slider1->setFunction(setVolume);
    Slider* slider2 = new Slider(renderer, 150, 200, 32, 8, 100, buttonTextPath);
    slider2->setFunction(setEnvLvl);
    // AR Sliders
    Slider* slider3 = new Slider(renderer, 250, 200, 32, 8, 100, buttonTextPath);
    slider3->setFunction(setAR_Attack);
    Slider* slider4 = new Slider(renderer, 300, 200, 32, 8, 100, buttonTextPath);
    slider4->setFunction(setAR_Release);
    // ADSR Sliders
    Slider* slider5 = new Slider(renderer, 400, 200, 32, 8, 100, buttonTextPath);
    slider5->setFunction(setADSR_Attack);
    Slider* slider6 = new Slider(renderer, 450, 200, 32, 8, 100, buttonTextPath);
    slider6->setFunction(setADSR_Decay);
    Slider* slider7 = new Slider(renderer, 500, 200, 32, 8, 100, buttonTextPath);
    slider7->setFunction(setADSR_Sustain);
    Slider* slider8 = new Slider(renderer, 550, 200, 32, 8, 100, buttonTextPath);
    slider8->setFunction(setADSR_Release);
    // ENV selector
    Button* button4 = new Button(renderer, 250, 150, 50, 50, buttonPurple);
    button4->setFunction(switchEnvType);
    //Filter 
    Slider* slider9 = new Slider(renderer, 550, 50, 32, 8, 100, buttonTextPath);
    slider9->setFunction(setCutoff);
    Slider* slider10 = new Slider(renderer, 600, 50, 32, 8, 100, buttonTextPath);
    slider10->setFunction(setResonance);

    // Osc
    Slider* slider11 = new Slider(renderer, 350, 50, 32, 8, 100, buttonTextPath);
    slider11->setFunction(setPulseWidth);
    Button* button5 = new Button(renderer, 250, 75, 64, 16, buttonTextPath);
    button5->setFunction(switchWaveForm);
    Slider* slider12 = new Slider(renderer, 150, 350, 32, 8, 100, buttonTextPath);
    slider12->setFunction(setFreqMod);
    Slider* slider13 = new Slider(renderer, 200, 350, 32, 8, 100, buttonTextPath);
    slider13->setFunction(setPwMod);

    //LFO
    Slider* slider14 = new Slider(renderer, 100, 350, 32, 8, 100, buttonTextPath);
    slider14->setFunction(setLfoRate);

    Slider* slider15 = new Slider(renderer, 300, 350, 32, 8, 100, buttonTextPath);
    slider15->setFunction(setDetune);

    Widgets.push_back(button1);
    Widgets.push_back(button2);
    Widgets.push_back(button3);
    Widgets.push_back(slider1);
    Widgets.push_back(slider2);
    Widgets.push_back(slider3);
    Widgets.push_back(slider4);
    Widgets.push_back(slider5);
    Widgets.push_back(slider6);
    Widgets.push_back(slider7);
    Widgets.push_back(slider8);
    Widgets.push_back(button4);
    Widgets.push_back(slider9);
    Widgets.push_back(slider10);
    Widgets.push_back(slider11);
    Widgets.push_back(button5);
    Widgets.push_back(slider12);
    Widgets.push_back(slider13);
    Widgets.push_back(slider14);
    Widgets.push_back(slider15);
}


///////// SYNTH CONTROLS /////////
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
//OSC
void SetPulseWidth(double coef){
    Oscillators[0]->setPulseWidth(coef);
    Oscillators[1]->setPulseWidth(coef);
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

void SetResonance(double coef){
    __Filter__->setResonance(coef);
}

void SwitchWaveForm(){
    Oscillators[0]->switchWaveForm();
    Oscillators[1]->switchWaveForm();
}

void SetFreqMod(double coef){
    Oscillators[0]->setLFO_freq_intensity(coef);
    Oscillators[1]->setLFO_freq_intensity(coef);
}
void SetPwMod(double coef){
    Oscillators[0]->setLFO_Pw_intensity(coef);
    Oscillators[1]->setLFO_Pw_intensity(coef);
}
void SetLfoRate(double coef){
    std::cout << "set Lfo Rate" << std::endl;
    __LFO__->setFreq(coef);
    std::cout << "set Lfo Rate" << std::endl;
}

void SetDetune(double coef){
    Oscillators[1]->setDetune(coef);
}