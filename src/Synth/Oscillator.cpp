#include "Oscillator.h"
#include "keyboard.h"
#include <iostream>

Oscillator::Oscillator(){
    Frequency = 1000.0;
    type = "saw";
    sampleRate = 44100;
}

Oscillator::~Oscillator(){

}

void Oscillator::init(std::string _type, unsigned long _sampleRate){
    type = _type;
    sampleRate = _sampleRate;
    Frequency = 1000.0;
}

void Oscillator::init(std::string _type, unsigned long _sampleRate, Keyboard* _keyboard){
    type = _type;
    sampleRate = _sampleRate;
    Frequency = 1000.0;
    keyboard = _keyboard;
}

void Oscillator::generateWave(PaData* Data){
    Frequency = keyboard->getNote();
    if(type == "saw"){
        Data->left_phase += Frequency/(double)sampleRate;
        if( Data->left_phase >= 1.0f ) Data->left_phase -= 2.0f;
        Data->right_phase += Frequency/(double)sampleRate;
        if( Data->right_phase >= 1.0f ) Data->right_phase -= 2.0f; 
    }else if(type == "sqr"){
        if(Data->t < (double)sampleRate/(2.0*Frequency)){
            Data->left_phase = 1.0f;
            Data->right_phase = 1.0f;
        }else{
            Data->left_phase = -1.0f;
            Data->right_phase = -1.0f;
        }
    }
    
    if((Data->t) < (double)sampleRate/Frequency){
        Data->t++;
    }else{
        Data->t = 0;
    }  
}

void Oscillator::setFreq(double coef){
    Frequency = coef*(maxFreq - minFreq);
    std::cout << "Osc Frequency : " << Frequency << std::endl;
}