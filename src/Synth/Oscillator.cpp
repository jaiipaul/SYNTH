#include "Oscillator.h"
#include "LFO.h"
#include "keyboard.h"
#include <iostream>
#include <math.h>

Oscillator::Oscillator(){
    Frequency = 1000.0;
    waveForm = 0;
    sampleRate = 44100;
}

Oscillator::~Oscillator(){

}

void Oscillator::init(int _waveForm, int _octave, unsigned long _sampleRate){
    waveForm = _waveForm;
    sampleRate = (float)_sampleRate;
    octave = _octave;
    baseFrequency =  pow(2.0f, 1.0f/12.0f) * pow(2, octave);
    detune = 0;
    PulseWidth = 0.5f;
    oscLFO_linked = false;
    time = 0;
}

void Oscillator::init(int _waveForm, int _octave, unsigned long _sampleRate, Keyboard* _keyboard){
    waveForm = _waveForm;
    sampleRate = (float)_sampleRate;
    octave = _octave;
    baseFrequency =  pow(2.0f, 1.0f/12.0f) * pow(2, octave);
    detune = 0;
    PulseWidth = 0.5f;
    keyboard = _keyboard;
    oscLFO_linked = false;
    time = 0;
}

float Oscillator::generateWave(){
    float value;
    float tempFreq = (baseFrequency*keyboard->getNote()*pow(2.f, detune/12.f));
    Frequency = oscLFO_linked ? freqLFO_intensity*maxFreqMod*oscLFO->getValue() + tempFreq : tempFreq;
    switch(waveForm){
        case 0: /*SAW*/
            value = time*2.f-1.f;
            break;
        case 1: /*TRIANGLE*/
            value = 1.f-fabs(time-0.5)*4.f;
            break;
        case 2: /*SQUARE*/
            PulseWidthMod = oscLFO_linked ? (PwLFO_intensity*maxPwMod*oscLFO->getValue() + PulseWidth) : PulseWidth;
            value  = (time < PulseWidthMod) ? 1.f : -1.f;
            break;
        case 3: /*SINE*/
            value = sin(2.f*PI*time);
    }

     if(t < sampleRate/Frequency){
        t++;
     }else{
        t = 0;
     }
    time = fmod(Frequency*(float)t/sampleRate,1.0);
    return(value);  
}

void Oscillator::setLFO(LFO* _oscLFO){
    oscLFO = _oscLFO;
    oscLFO_linked = true;
}
void Oscillator::setLFO_freq_intensity(double coef){
    freqLFO_intensity = (float)coef;
    std::cout << "Osc LFO Frequency intensity : " << freqLFO_intensity << std::endl;
}
void Oscillator::setLFO_Pw_intensity(double coef){
    PwLFO_intensity = (float)coef;
    std::cout << "Osc LFO PulseWidth intensity : " << PwLFO_intensity << std::endl;
}

void Oscillator::switchWaveForm(){
    waveForm = (++waveForm)%4;
    switch(waveForm){
        case 0:
            std::cout << "Oscillator WaveForm : SAW" << std::endl;
            break;
        case 1:
            std::cout << "Oscillator WaveForm : TRI" << std::endl;
            break;
        case 2:
            std::cout << "Oscillator WaveForm : SQR" << std::endl;
            break;
        case 3:
            std::cout << "Oscillator WaveForm : SINE" << std::endl;
            break;
    }
}

void Oscillator::setWaveForm(int form){
    waveForm = form;
    switch(waveForm){
        case 0:
            std::cout << "Oscillator WaveForm : SAW" << std::endl;
            break;
        case 1:
            std::cout << "Oscillator WaveForm : TRI" << std::endl;
            break;
        case 2:
            std::cout << "Oscillator WaveForm : SQR" << std::endl;
            break;
        case 3:
            std::cout << "Oscillator WaveForm : SINE" << std::endl;
            break;
    }
}

void Oscillator::setBaseFreq(double coef){
    Frequency = coef*(maxFreq - minFreq);
    std::cout << "Osc Frequency : " << Frequency << std::endl;
}

void Oscillator::setPulseWidth(double coef){
    PulseWidth = 0.05f + (float)coef*(0.90f);
    std::cout << "Osc PulseWidth : " << PulseWidth << std::endl;
}

void Oscillator::setDetune(double coef){
    detune =  (float)coef*(12.f);
    std::cout << "Osc Detune : " << detune << std::endl;
}