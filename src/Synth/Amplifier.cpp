#include "Amplifier.h"
#include "keyboard.h"
#include "Envelops.h"
#include <iostream>

Amplifier::Amplifier(){

}
Amplifier::~Amplifier(){

}

void Amplifier::init(unsigned long sampleRate, Keyboard* _keyboard){
    keyboard = _keyboard;
    volume   = 1.0f;
    envelop_level = 1.0f;

    EnvType = 0;
    AR = new AR_envelop();
    AR->init(sampleRate, _keyboard);
    ADSR = new ADSR_envelop();
    ADSR->init(sampleRate, _keyboard);
}
    
void Amplifier::genOutput(PaData* Data){
    switch(EnvType){
        case 0: /*GATE*/
            if(keyboard->getGate()){
                Data->left_phase  = volume*Data->left_phase;
                Data->right_phase = volume*Data->right_phase;
            }else{
                Data->left_phase  = 0.0;
                Data->right_phase = 0.0;
            } 
            break;
        
        case 1: /*AR*/
            AR->Update();
            Data->left_phase  = AR->getAR_Coef()*envelop_level*volume*Data->left_phase;
            Data->right_phase = AR->getAR_Coef()*envelop_level*volume*Data->right_phase;
            break;
        
        case 2: /*ADSR*/
            ADSR->Update();
            Data->left_phase  = ADSR->getADSR_Coef()*envelop_level*volume*Data->left_phase;
            Data->right_phase = ADSR->getADSR_Coef()*envelop_level*volume*Data->right_phase;
            break;

        default:
            break;
    }
}

void Amplifier::setVolume(double coef){
    volume = coef <= 1.0f ? coef : 1.0f;
    std::cout << "Amp volume : " << volume << std::endl;
}

void Amplifier::setEnvelopLevel(double coef){
    envelop_level = coef <= 1.0f ? coef : 1.0f;
    std::cout << "Amp envelop level : " << envelop_level << std::endl;
}

void Amplifier::switchEnvelopType(){
    EnvType = (++EnvType)%3;
    switch(EnvType){
        case 0:
            std::cout << "Amp envelop type : Gate" << std::endl;
            break;
        case 1:
            std::cout << "Amp envelop type : AR" << std::endl;
            break;
        case 2:
            std::cout << "Amp envelop type : ADSR" << std::endl;
            break;
    } 
}

void Amplifier::setEnvelopType(int type){
    EnvType = type;
    switch(EnvType){
        case 0:
            std::cout << "Amp envelop type : Gate" << std::endl;
            break;
        case 1:
            std::cout << "Amp envelop type : AR" << std::endl;
            break;
        case 2:
            std::cout << "Amp envelop type : ADSR" << std::endl;
            break;
    } 
}