#include <vector>

#include "Synth.h"
#include "../GUI/Widgets.h"
#include "../Synth/Modules.h"
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
static Oscillator* __SubOsc__ = new Oscillator();

/* VCA */
static Amplifier*  __Amp__ = new Amplifier();

/* VCF */
static LadderFilter* __Filter__ = new LadderFilter();

/* LFOs */ 
static std::vector<LFO*> LFOs;
static LFO* __LFO1__ = new LFO();
static LFO* __LFO2__ = new LFO();

/* ENVELOPS */
static std::vector<AR_envelop*> ARs;
static AR_envelop*  __AR1__ = new AR_envelop();
static AR_envelop*  __AR2__ = new AR_envelop();

static std::vector<ADSR_envelop*> ADSRs;
static ADSR_envelop*  __ADSR1__ = new ADSR_envelop();
static ADSR_envelop*  __ADSR2__ = new ADSR_envelop();

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

        SynthOutput(__Mix__, __Filter__, __Amp__, LFOs, ARs, ADSRs, callbackData);
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
    __gui__ = new Gui();
    name = _name; width = _width; height = _height;
    __gui__->init(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, fullscreen);

    buildSynthGui();
    std::cout << "Built Gui..." << std::endl;
    buildSynthModules();
    std::cout << "Built Modules..." << std::endl;
}

bool Synth::isRunning(){
    return(__gui__->isRunning());
}

void Synth::handleEvents(){
    __gui__->handleEvents();
}
void Synth::update(){
    __gui__->update();
    __keyboard__->Update();
}   

void Synth::render(){
    __gui__->render();
}

void Synth::clean(){
    __audioEngine__->clean();
    __gui__->clean();
    std::cout << "Synth cleaned!..." << std::endl;
}

/////// SYNTH MODULES INITILISATION ////////
void Synth::buildSynthModules(){
    __keyboard__ = new Keyboard();
    __keyboard__->init(true);

    __SubOsc__->init(2, 2, sampleRate, __keyboard__);
    __Osc1__->init(2, 3, sampleRate, __keyboard__);
    __Osc2__->init(1, 3, sampleRate, __keyboard__);
    
    Oscillators.push_back(__SubOsc__);
    Oscillators.push_back(__Osc1__);
    Oscillators.push_back(__Osc2__);
    
    __Mix__->init(3);
    __Amp__->init(sampleRate, __keyboard__);
    __Filter__->init(96000);

    __AR1__->init(sampleRate, __keyboard__);
    __AR2__->init(sampleRate, __keyboard__);
    __ADSR1__->init(sampleRate, __keyboard__);
    __ADSR2__->init(sampleRate, __keyboard__);

    ARs.push_back(__AR1__);
    ARs.push_back(__AR2__);
    ADSRs.push_back(__ADSR1__);
    ADSRs.push_back(__ADSR2__);

    __LFO1__->init(3, sampleRate, __keyboard__);
    __LFO2__->init(3, sampleRate, __keyboard__);
    LFOs.push_back(__LFO1__);
    LFOs.push_back(__LFO2__);

    /* Bindings */
    __Osc1__->bindLFO(LFOs[0]);
    __Osc2__->bindLFO(LFOs[0]);
    
    __Amp__->bindAR(ARs[0]);
    __Amp__->bindADSR(ADSRs[0]);

    __Filter__->bindLFO(LFOs[1]);
    __Filter__->bindAR(ARs[1]);
    __Filter__->bindADSR(ADSRs[1]);
}

/////// SYNTH SOUND PATH ///////
void Synth::SynthOutput(Mixer* Mix, LadderFilter* Filt, Amplifier* Amp, 
                        std::vector<LFO*> _LFOs, 
                        std::vector<AR_envelop*> _ARs, 
                        std::vector<ADSR_envelop*> _ADSRs, 
                        PaData* Data){
    
    for(auto lfo : _LFOs){
        lfo->generateWave();
    }
    for(auto ar : _ARs){
        ar->Update();
    }
    for(auto adsr : _ADSRs){
        adsr->Update();
    }

    __Mix__->generateOutput(Data, Oscillators);
    __Filter__->genOutput(Data);
    __Amp__->genOutput(Data);
}

////////  SYNTH GUI /////////
void Synth::buildSynthGui(){
    /* AUDIO ENGINE */
    buttonFunc startStream     = &(StartStream);
    buttonFunc stopStream      = &(StopStream);
    buttonFunc listDevices     = &(ListDevices);
    /* OSC 1 (and sub)*/
    buttonFunc switchOSC1WaveForm  = &(SwitchOSC1WaveForm);
    sliderFunc setPulseWidth1      = &(SetPulseWidth1);
    sliderFunc setFreqMod1         = &(SetFreqMod1);
    sliderFunc setPwMod1           = &(SetPwMod1);

    /* OSC 2 */
    buttonFunc switchOSC2WaveForm  = &(SwitchOSC2WaveForm);
    sliderFunc setPulseWidth2      = &(SetPulseWidth2);
    sliderFunc setFreqMod2         = &(SetFreqMod2);
    sliderFunc setPwMod2           = &(SetPwMod2);
    sliderFunc setDetune2          = &(SetDetune2);

    /* MIXER */
    sliderFunc setVolumeSub    = &(SetVolumeSub);
    sliderFunc setVolume1      = &(SetVolume1);
    sliderFunc setVolume2      = &(SetVolume2);
    
    /* VCA */
    sliderFunc setGlobalVolume  = &(SetGlobalVolume);
    sliderFunc setVCAEnvLvl     = &(SetVCAEnvLvl);
    buttonFunc switchEnvTypeVCA = &(SwitchEnvTypeVCA);

    /* ENVELOPS */
    sliderFunc setAR_Attack1    = &(SetAR_Attack1);
    sliderFunc setAR_Release1   = &(SetAR_Release1);
    sliderFunc setADSR_Attack1  = &(SetADSR_Attack1);
    sliderFunc setADSR_Decay1   = &(SetADSR_Decay1);
    sliderFunc setADSR_Sustain1 = &(SetADSR_Sustain1);
    sliderFunc setADSR_Release1 = &(SetADSR_Release1);

    sliderFunc setAR_Attack2    = &(SetAR_Attack2);
    sliderFunc setAR_Release2   = &(SetAR_Release2);
    sliderFunc setADSR_Attack2  = &(SetADSR_Attack2);
    sliderFunc setADSR_Decay2   = &(SetADSR_Decay2);
    sliderFunc setADSR_Sustain2 = &(SetADSR_Sustain2);
    sliderFunc setADSR_Release2 = &(SetADSR_Release2);

    /* FILTER */
    buttonFunc switchEnvTypeVCF = &(SwitchEnvTypeVCF);
    sliderFunc setCutoff        = &(SetCutoff);
    sliderFunc setResonance     = &(SetResonance);
    sliderFunc setDrive         = &(SetDrive);
    sliderFunc setVCF_LFOint    = &(SetVCF_LFOint);
    sliderFunc setVCFEnvLvl     = &(SetVCFEnvLvl);

    /* LFOs */
    sliderFunc setLFO1Rate      = &(SetLFO1Rate);
    sliderFunc setLFO2Rate      = &(SetLFO2Rate);
    buttonFunc switchLFO1WaveForm = &(SwitchLFO1WaveForm);
    buttonFunc switchLFO2WaveForm = &(SwitchLFO2WaveForm);

    const char* buttonTextPath = "../../../assets/button.png";
    const char* buttonPurple = "../../../assets/button_purple.png";

    Button* button1 = new Button(__gui__, 10, 10, 64, 16, buttonTextPath);
    button1->bind(startStream);
    Button* button2 = new Button(__gui__, 10, 36, 64, 16, buttonTextPath);
    button2->bind(stopStream);
    Button* button3 = new Button(__gui__, 10, 62, 64, 16, buttonTextPath);
    button3->bind(listDevices);
    // AMP sliders
    Slider* slider1 = new Slider(__gui__, 100, 200, 32, 8, 100, buttonTextPath);
    slider1->bind(setGlobalVolume);
    Slider* slider2 = new Slider(__gui__, 150, 200, 32, 8, 100, buttonTextPath);
    slider2->bind(setVCAEnvLvl);
    // AR Sliders
    Slider* slider3 = new Slider(__gui__, 250, 200, 32, 8, 100, buttonTextPath);
    slider3->bind(setAR_Attack1);
    Slider* slider4 = new Slider(__gui__, 300, 200, 32, 8, 100, buttonTextPath);
    slider4->bind(setAR_Release1);
    // ADSR Sliders
    Slider* slider5 = new Slider(__gui__, 400, 200, 32, 8, 100, buttonTextPath);
    slider5->bind(setADSR_Attack1);
    Slider* slider6 = new Slider(__gui__, 450, 200, 32, 8, 100, buttonTextPath);
    slider6->bind(setADSR_Decay1);
    Slider* slider7 = new Slider(__gui__, 500, 200, 32, 8, 100, buttonTextPath);
    slider7->bind(setADSR_Sustain1);
    Slider* slider8 = new Slider(__gui__, 550, 200, 32, 8, 100, buttonTextPath);
    slider8->bind(setADSR_Release1);
    // ENV selector
    Button* button4 = new Button(__gui__, 250, 150, 50, 50, buttonPurple);
    button4->bind(switchEnvTypeVCA);
    //Filter 
    Slider* slider9 = new Slider(__gui__, 550, 50, 32, 8, 100, buttonTextPath);
    slider9->bind(setCutoff);
    Slider* slider10 = new Slider(__gui__, 600, 50, 32, 8, 100, buttonTextPath);
    slider10->bind(setResonance);

    // Osc
    Slider* slider11 = new Slider(__gui__, 350, 50, 32, 8, 100, buttonTextPath);
    slider11->bind(setPulseWidth1);
    Button* button5 = new Button(__gui__, 250, 75, 64, 16, buttonTextPath);
    button5->bind(switchOSC1WaveForm);
    Slider* slider12 = new Slider(__gui__, 150, 350, 32, 8, 100, buttonTextPath);
    slider12->bind(setFreqMod1);
    Slider* slider13 = new Slider(__gui__, 200, 350, 32, 8, 100, buttonTextPath);
    slider13->bind(setPwMod1);

    //LFO
    Slider* slider14 = new Slider(__gui__, 100, 350, 32, 8, 100, buttonTextPath);
    slider14->bind(setLFO1Rate);

    Slider* slider15 = new Slider(__gui__, 300, 350, 32, 8, 100, buttonTextPath);
    slider15->bind(setDetune2);

    __gui__->addWidget(button1);
    __gui__->addWidget(button2);
    __gui__->addWidget(button3);
    __gui__->addWidget(slider1);
    __gui__->addWidget(slider2);
    __gui__->addWidget(slider3);
    __gui__->addWidget(slider4);
    __gui__->addWidget(slider5);
    __gui__->addWidget(slider6);
    __gui__->addWidget(slider7);
    __gui__->addWidget(slider8);
    __gui__->addWidget(button4);
    __gui__->addWidget(slider9);
    __gui__->addWidget(slider10);
    __gui__->addWidget(slider11);
    __gui__->addWidget(button5);
    __gui__->addWidget(slider12);
    __gui__->addWidget(slider13);
    __gui__->addWidget(slider14);
    __gui__->addWidget(slider15);
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
void SetPulseWidth1(double coef){
    Oscillators[1]->setPulseWidth(coef);
}
// AMPLIFIER
void SetGlobalVolume(double coef){
    __Amp__->setVolume(coef);
}
void SetVCAEnvLvl(double coef){
    __Amp__->setEnvelopLevel(coef);
}

// AR AMP ENVELOP
void SetAR_Attack1(double coef){
    ARs[0]->setAttack(coef);
}
void SetAR_Release1(double coef){
    ARs[0]->setRelease(coef);
}

// ADSR AMP ENVELOP
void SetADSR_Attack1(double coef){
    ADSRs[0]->setAttack(coef);
}
void SetADSR_Decay1(double coef){
    ADSRs[0]->setDecay(coef);
}
void SetADSR_Sustain1(double coef){
    ADSRs[0]->setSustain(coef);
}
void SetADSR_Release1(double coef){
    ADSRs[0]->setRelease(coef);
}

void SwitchEnvTypeVCA(){
    __Amp__->switchEnvelopType();
}

void SetCutoff(double coef){
    __Filter__->setCutoff(coef);
}

void SetResonance(double coef){
    __Filter__->setResonance(coef);
}

void SwitchOSC1WaveForm(){
    Oscillators[1]->switchWaveForm();
}

void SetFreqMod1(double coef){
    Oscillators[1]->setLFO_freq_intensity(coef);
}
void SetPwMod1(double coef){
    Oscillators[1]->setLFO_Pw_intensity(coef);
}
void SetLFO1Rate(double coef){
    std::cout << "set Lfo Rate" << std::endl;
    __LFO1__->setFreq(coef);
    std::cout << "set Lfo Rate" << std::endl;
}

void SetDetune2(double coef){
    Oscillators[2]->setDetune(coef);
}