#include <vector>

#include "Synth.h"
#include "../GUI/Widgets.h"
#include "../Synth/Modules.h"
#include "SynthControls.h"
#include "../GUI/textures.h"

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
        float L = callbackData->left_phase;
        float R = callbackData->right_phase;
        //(abs(L)  > 1.f)  ? printf("CLIP\n") : 0; 
        //std::cout << callbackData->left_phase << std::endl;
        out[2*i]   = (abs(L) < 1.f)  ? L  : (L > 0) - (L < 0);  /* left */
        out[2*i+1] = (abs(R) < 1.f)  ? R  : (R > 0) - (R < 0);  /* right */
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

    __audioEngine__->StartStream();
    std::cout << "stream started..." << std::endl;
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

    // AMP sliders
    Slider* VCA_volumeSlider = new Slider(__gui__, 392, 196, 32, 16, 100, sliderTextPath);
    VCA_volumeSlider->bind(setGlobalVolume);
    Slider* VCA_envLvlSlider = new Slider(__gui__, 432, 196, 32, 16, 100, sliderTextPath);
    VCA_envLvlSlider->bind(setVCAEnvLvl);
    Button* VCA_envButton = new Button(__gui__, 486, 196, 32, 32, buttonTextPath);
    VCA_envButton->bind(switchEnvTypeVCA);

    // AR Sliders
    Slider* AR1_attackSlider = new Slider(__gui__, 656, 196, 32, 16, 100, sliderTextPath);
    AR1_attackSlider->bind(setAR_Attack1);
    Slider* AR1_releaseSlider = new Slider(__gui__, 696, 196, 32, 16, 100, sliderTextPath);
    AR1_releaseSlider->bind(setAR_Release1);
    Slider* AR2_attackSlider = new Slider(__gui__, 656, 64, 32, 16, 100, sliderTextPath);
    AR2_attackSlider->bind(setAR_Attack2);
    Slider* AR2_releaseSlider = new Slider(__gui__, 696, 64, 32, 16, 100, sliderTextPath);
    AR2_releaseSlider->bind(setAR_Release2);
    // ADSR Sliders
    Slider* ADSR1_attackSlider = new Slider(__gui__, 740, 196, 32, 16, 100, sliderTextPath);
    ADSR1_attackSlider->bind(setADSR_Attack1);
    Slider* ADSR1_decaySlider = new Slider(__gui__, 780, 196, 32, 16, 100, sliderTextPath);
    ADSR1_decaySlider->bind(setADSR_Decay1);
    Slider* ADSR1_sustainSlider = new Slider(__gui__, 820, 196, 32, 16, 100, sliderTextPath);
    ADSR1_sustainSlider->bind(setADSR_Sustain1);
    Slider* ADSR1_releaseSlider = new Slider(__gui__, 860, 196, 32, 16, 100, sliderTextPath);
    ADSR1_releaseSlider->bind(setADSR_Release1);

    Slider* ADSR2_attackSlider = new Slider(__gui__, 740, 64, 32, 16, 100, sliderTextPath);
    ADSR2_attackSlider->bind(setADSR_Attack2);
    Slider* ADSR2_decaySlider = new Slider(__gui__, 780, 64, 32, 16, 100, sliderTextPath);
    ADSR2_decaySlider->bind(setADSR_Decay2);
    Slider* ADSR2_sustainSlider = new Slider(__gui__, 820, 64, 32, 16, 100, sliderTextPath);
    ADSR2_sustainSlider->bind(setADSR_Sustain2);
    Slider* ADSR2_releaseSlider = new Slider(__gui__, 860, 64, 32, 16, 100, sliderTextPath);
    ADSR2_releaseSlider->bind(setADSR_Release2);
    
    
    //Filter 
    Slider* VCF_cutoffSlider = new Slider(__gui__, 392, 64, 32, 16, 100, sliderTextPath);
    VCF_cutoffSlider->bind(setCutoff);
    Slider* VCF_resoSlider = new Slider(__gui__, 432, 64, 32, 16, 100, sliderTextPath);
    VCF_resoSlider->bind(setResonance);
    Slider* VCF_driveSlider = new Slider(__gui__, 472, 64, 32, 16, 100, sliderTextPath);
    VCF_driveSlider->bind(setDrive);
    Button* VCF_EnvButton = new Button(__gui__, 608, 64, 32, 32, buttonTextPath);
    VCF_EnvButton->bind(switchEnvTypeVCF);
    Slider* VCF_envLvlSlider = new Slider(__gui__, 520, 64, 32, 16, 100, sliderTextPath);
    VCF_envLvlSlider->bind(setVCFEnvLvl);
    Slider* VCF_lfoIntSlider = new Slider(__gui__, 560, 64, 32, 16, 100, sliderTextPath);
    VCF_lfoIntSlider->bind(setVCF_LFOint);
    

    //LFOs
    Slider* LFO1_rateSlider = new Slider(__gui__, 328, 196, 32, 16, 100, sliderTextPath);
    LFO1_rateSlider->bind(setLFO1Rate);
    Button* LFO1_waveformButton = new Button(__gui__, 288, 196, 32, 32, buttonTextPath);
    LFO1_waveformButton->bind(switchLFO1WaveForm);
    Slider* LFO2_rateSlider = new Slider(__gui__, 328, 328, 32, 16, 100, sliderTextPath);
    LFO2_rateSlider->bind(setLFO2Rate);
    Button* LFO2_waveformButton = new Button(__gui__, 288, 328, 32, 32, buttonTextPath);
    LFO2_waveformButton->bind(switchLFO2WaveForm);

    // MIXER
    Slider* MIX_volumeSubSlider = new Slider(__gui__, 248, 64, 32, 16, 100, sliderTextPath);
    MIX_volumeSubSlider->bind(setVolumeSub);
    Slider* MIX_volume1Slider = new Slider(__gui__, 288, 64, 32, 16, 100, sliderTextPath);
    MIX_volume1Slider->bind(setVolume1);
    Slider* MIX_volume2Slider = new Slider(__gui__, 328, 64, 32, 16, 100, sliderTextPath);
    MIX_volume2Slider->bind(setVolume2);

    // OSCILLATORS
    Button* OSC1_waveformButton = new Button(__gui__, 32, 64, 32, 32, buttonTextPath);
    OSC1_waveformButton->bind(switchOSC1WaveForm);
    Slider* OSC1_pwSlider = new Slider(__gui__, 80, 64, 32, 16, 100, sliderTextPath);
    OSC1_pwSlider->bind(setPulseWidth1);
    Slider* OSC1_freqmodSlider = new Slider(__gui__, 120, 64, 32, 16, 100, sliderTextPath);
    OSC1_freqmodSlider->bind(setFreqMod1);
    Slider* OSC1_pwmodSlider = new Slider(__gui__, 160, 64, 32, 16, 100, sliderTextPath);
    OSC1_pwmodSlider->bind(setPwMod1);

    Button* OSC2_waveformButton = new Button(__gui__, 32, 196, 32, 32, buttonTextPath);
    OSC2_waveformButton->bind(switchOSC2WaveForm);
    Slider* OSC2_pwSlider = new Slider(__gui__, 80, 196, 32, 16, 100, sliderTextPath);
    OSC2_pwSlider->bind(setPulseWidth2);
    Slider* OSC2_freqmodSlider = new Slider(__gui__, 120, 196, 32, 16, 100, sliderTextPath);
    OSC2_freqmodSlider->bind(setFreqMod2);
    Slider* OSC2_pwmodSlider = new Slider(__gui__, 160, 196, 32, 16, 100, sliderTextPath);
    OSC2_pwmodSlider->bind(setPwMod2);
    Slider* OSC2_detuneSlider = new Slider(__gui__, 200, 196, 32, 16, 100, sliderTextPath);
    OSC2_detuneSlider->bind(setDetune2);
}


///////// SYNTH CONTROLS /////////

// OSCS
// OSC 1 controls
void SwitchOSC1WaveForm(){
    Oscillators[1]->switchWaveForm();
}
void SetPulseWidth1(double coef){
    Oscillators[1]->setPulseWidth(coef);
}
void SetFreqMod1(double coef){
    Oscillators[1]->setLFO_freq_intensity(coef);
}
void SetPwMod1(double coef){
    Oscillators[1]->setLFO_Pw_intensity(coef);
}


// OSC 2 controls
void SwitchOSC2WaveForm(){
    Oscillators[2]->switchWaveForm();
}
void SetPulseWidth2(double coef){
    Oscillators[2]->setPulseWidth(coef);
}
void SetFreqMod2(double coef){
    Oscillators[2]->setLFO_freq_intensity(coef);
}
void SetPwMod2(double coef){
    Oscillators[2]->setLFO_Pw_intensity(coef);
}
void SetDetune2(double coef){
    Oscillators[2]->setDetune(coef);
}

// MIXER controls
void SetVolumeSub(double coef){
    __Mix__->setVolume(0, coef);
}
void SetVolume1(double coef){
    __Mix__->setVolume(1, coef);
}
void SetVolume2(double coef){
    __Mix__->setVolume(2, coef);
}

// VCA controls
void SetGlobalVolume(double coef){
    __Amp__->setVolume(coef);
}
void SetVCAEnvLvl(double coef){
    __Amp__->setEnvelopLevel(coef);
}
void SwitchEnvTypeVCA(){
    __Amp__->switchEnvelopType();
}

// ENVELOPS controls
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

void SetAR_Attack2(double coef){
    ARs[1]->setAttack(coef);
}
void SetAR_Release2(double coef){
    ARs[1]->setRelease(coef);
}

// ADSR AMP ENVELOP
void SetADSR_Attack2(double coef){
    ADSRs[1]->setAttack(coef);
}
void SetADSR_Decay2(double coef){
    ADSRs[1]->setDecay(coef);
}
void SetADSR_Sustain2(double coef){
    ADSRs[1]->setSustain(coef);
}
void SetADSR_Release2(double coef){
    ADSRs[1]->setRelease(coef);
}

// FILTER controls
void SetCutoff(double coef){
    __Filter__->setCutoff(coef);
}

void SetResonance(double coef){
    __Filter__->setResonance(coef);
}
void SetDrive(double coef){
    __Filter__->setDrive(coef);
}
void SwitchEnvTypeVCF(){
    __Filter__->switchEnvType();
}
void SetVCFEnvLvl(double coef){
    __Filter__->setENVintensity(coef);
}
void SetVCF_LFOint(double coef){
    __Filter__->setLFOintensity(coef);
}


// LFOs controls
void SetLFO1Rate(double coef){
    __LFO1__->setFreq(coef);
}
void SetLFO2Rate(double coef){
    __LFO2__->setFreq(coef);
}
void SwitchLFO1WaveForm(){
    __LFO1__->switchWaveForm();
}
void SwitchLFO2WaveForm(){
    __LFO2__->switchWaveForm();
}





