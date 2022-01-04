#include "Oscillator.h"
#include "LFO.h"
#include "keyboard.h"
#include <iostream>

Oscillator::Oscillator(){
    Frequency = 1000.0;
    waveForm = 0;
    sampleRate = 44100;
}

Oscillator::~Oscillator(){

}

void Oscillator::init(int _waveForm, unsigned long _sampleRate){
    waveForm = _waveForm;
    sampleRate = _sampleRate;
    Frequency = 1000.0;
    PulseWidth = 0.5f;
    oscLFO_linked = false;
}

void Oscillator::init(int _waveForm, unsigned long _sampleRate, Keyboard* _keyboard){
    waveForm = _waveForm;
    sampleRate = _sampleRate;
    Frequency = 1000.0;
    PulseWidth = 0.5f;
    keyboard = _keyboard;
    oscLFO_linked = false;
}

void Oscillator::generateWave(PaData* Data){
    Frequency = oscLFO_linked ? freqLFO_intensity*maxFreqMod*oscLFO->getValue() + keyboard->getNote() : keyboard->getNote();
    //std::cout << "Osc Frequency : " << Frequency << std::endl;
    switch(waveForm){
        case 0: /*SAW*/
            Data->left_phase = -1.f + (float)Data->t * 2.f*Frequency/(float)sampleRate;
            Data->right_phase = -1.f + (float)Data->t * 2.f*Frequency/(float)sampleRate;
            break;
        case 1: /*TRIANGLE*/
            if(Data->t < (float)sampleRate/(2.f*Frequency)){
                Data->left_phase  = -1.f + (float)Data->t * 4.f*Frequency/(float)sampleRate;
                Data->right_phase = -1.f + (float)Data->t * 4.f*Frequency/(float)sampleRate;
            }else{
                Data->left_phase  = 1.f - (float)Data->t * 4.f*Frequency/(float)sampleRate;
                Data->right_phase = 1.f - (float)Data->t * 4.f*Frequency/(float)sampleRate;
            }
            break;
        case 2: /*SQUARE*/
            PulseWidth = oscLFO_linked ? (PwLFO_intensity*maxPwMod*oscLFO->getValue() + PulseWidth) : PulseWidth;
            if(Data->t < (PulseWidth*(float)sampleRate/Frequency)){
            Data->left_phase = 1.0f;
            Data->right_phase = 1.0f;
            }else{
            Data->left_phase = -1.0f;
            Data->right_phase = -1.0f;
            }
            break;
    }

    if((Data->t) < (float)sampleRate/Frequency){
        Data->t++;
    }else{
        Data->t = 0;
    }
    //std::cout << Data->left_phase << std::endl;  
}

void Oscillator::setLFO(LFO* _oscLFO){
    oscLFO = _oscLFO;
    oscLFO_linked = false;
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
    waveForm = (++waveForm)%3;
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
    }
}

void Oscillator::setFreq(double coef){
    Frequency = coef*(maxFreq - minFreq);
    std::cout << "Osc Frequency : " << Frequency << std::endl;
}

void Oscillator::setPulseWidth(double coef){
    PulseWidth = 0.05f + (float)coef*(0.90f);
    std::cout << "Osc PulseWidth : " << PulseWidth << std::endl;
}