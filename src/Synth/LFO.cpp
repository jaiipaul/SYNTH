#include "LFO.h"
#include "keyboard.h"
#include <iostream>

LFO::LFO(){
    Frequency = 1000.0;
    waveForm = 0;
    sampleRate = 44100;
}

LFO::~LFO(){

}

void LFO::init(int _waveForm, unsigned long _sampleRate){
    waveForm = _waveForm;
    sampleRate = _sampleRate;
    Frequency = 0.0;
    keySync = true;
    value = 0;
}

void LFO::init(int _waveForm, unsigned long _sampleRate, Keyboard* _keyboard){
    waveForm = _waveForm;
    sampleRate = _sampleRate;
    Frequency = 0.0;
    keySync  = true;
    value = 0;
    keyboard = _keyboard;
}

void LFO::generateWave(){
    if(keySync && keyboard->getTrigger()) t = 0;
    
    switch(waveForm){
        case 0: /*SAW*/
            value += Frequency/(double)sampleRate;
            if( value >= 1.0f ) value -= 2.0f;
            break;
        case 1: /*TRIANGLE*/
            if(t < (double)sampleRate/(4.f*Frequency)){
                value  += Frequency/(double)sampleRate;
            }else if(t >= (double)sampleRate/(4.f*Frequency) && 
                     t < (3.f*(double)sampleRate)/(4.f*Frequency)){
                value  -= Frequency/(double)sampleRate;
            }else{
                value  += Frequency/(double)sampleRate;
            }
            break;
        case 2: /*SQUARE*/
            if(t < (0.5f*(double)sampleRate/Frequency)){
            value = 1.0f;
            }else{
            value = -1.0f;
            }
            break;
    }

    if(t < (double)sampleRate/Frequency){
        t++;
    }else{
        t = 0;
    }  
}

float LFO::getValue(){
    return value;
}
void LFO::switchWaveForm(){
    waveForm = (++waveForm)%3;
    switch(waveForm){
        case 0:
            std::cout << "LFO WaveForm : SAW" << std::endl;
            break;
        case 1:
            std::cout << "LFO WaveForm : TRI" << std::endl;
            break;
        case 2:
            std::cout << "LFO WaveForm : SQR" << std::endl;
            break;
    }
}

void LFO::setWaveForm(int form){
    waveForm = form;
    switch(waveForm){
        case 0:
            std::cout << "LFO WaveForm : SAW" << std::endl;
            break;
        case 1:
            std::cout << "LFO WaveForm : TRI" << std::endl;
            break;
        case 2:
            std::cout << "LFO WaveForm : SQR" << std::endl;
            break;
    }
}

void LFO::setFreq(double coef){
    Frequency = coef*(maxFreq - minFreq);
    std::cout << "Osc Frequency : " << Frequency << std::endl;
}
