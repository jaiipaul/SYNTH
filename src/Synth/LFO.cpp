#include "LFO.h"
#include "keyboard.h"
#include <iostream>
#include <math.h>

LFO::LFO(){
    Frequency = 1000.0;
    waveForm = 0;
    sampleRate = 44100;
}

LFO::~LFO(){

}

void LFO::init(int _waveForm, unsigned long _sampleRate){
    waveForm = _waveForm;
    sampleRate = (float)_sampleRate;
    Frequency = 0.0;
    keySync = true;
    value = 0;
}

void LFO::init(int _waveForm, unsigned long _sampleRate, Keyboard* _keyboard){
    waveForm = _waveForm;
    sampleRate =(float) _sampleRate;
    Frequency = 0.0;
    keySync  = true;
    value = 0;
    keyboard = _keyboard;
}

void LFO::generateWave(){
    if(keySync && keyboard->getTrigger()) t = 0;
    
    switch(waveForm){
        case 0: /*SAW*/
            value  = time*2.f-1.f;
            break;
        case 1: /*TRIANGLE*/
            value  = 1.f-fabs(time-0.5)*4.f;
            break;
        case 2: /*SQUARE*/
            if(t < (sampleRate/Frequency)){
            value = 1.0f;
            }else{
            value = -1.0f;
            }
            break;
        case 3: /*SINE*/
            value = sin(2.f*PI*time);
    }

    // if((Data->t) < sampleRate/Frequency){
        // Data->t++;
    // }else{
        // Data->t = 0;
    // }
    t++;
    time = fmod(Frequency*(float)t/sampleRate,1.0);
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
