#include "Envelops.h"
#include "keyboard.h"
#include <iostream>

// Attack-Release Envelop //
AR_envelop::AR_envelop(){

}

AR_envelop::~AR_envelop(){

}

void AR_envelop::init(unsigned long _sampleRate, Keyboard* _keyboard){
    sampleRate = _sampleRate;
    attackEch  = maxAttack*sampleRate;
    releaseEch = maxRelease*sampleRate;

    ARcoef = 0.0f;

    keyboard = _keyboard;
}

void AR_envelop::Update(){
    if(keyboard->getTrigger()) ARcoef = 0.0f;
    if(keyboard->getGate()){
        //std::cout << Trigger << std::endl;
        ARcoef = (ARcoef < 1.0f && attackEch > 0) ? ARcoef + 1.0f/(float)attackEch : 1.0f;
    }else if(!keyboard->getGate()){
        ARcoef = (ARcoef > 0.0f && releaseEch > 0) ? ARcoef - (1.0f/(float)releaseEch) : 0.0f; 
    }
}

float AR_envelop::getAR_Coef(){
    return(ARcoef);
}

void AR_envelop::setAttack(double coef){
    attackEch = (coef > 0.0f && coef <= 1.0f) ? coef*maxAttack*sampleRate : 0.0f;
    std::cout << "AR attack : " << (float)attackEch / (float)sampleRate << std::endl;
}
void AR_envelop::setRelease(double coef){
    releaseEch = (coef > 0.0f && coef <= 1.0f) ? coef*maxRelease*sampleRate : 0.0f;
    std::cout << "AR release : " << (float)releaseEch / (float)sampleRate << std::endl;
}

// Attack-Decay-Sustain-Release Envelop //

ADSR_envelop::ADSR_envelop(){

}

ADSR_envelop::~ADSR_envelop(){

}

void ADSR_envelop::init(unsigned long _sampleRate, Keyboard* _keyboard){
    sampleRate = _sampleRate;
    attackEch  = maxAttack*sampleRate;
    decayEch   = maxDecay*sampleRate;
    releaseEch = maxRelease*sampleRate;

    t = 0;
    ADSRcoef = 0.0f;
    keyboard = _keyboard;
}

void ADSR_envelop::Update(){
    if(keyboard->getTrigger()){
        std::cout << "reset env" << std::endl;
        ADSRcoef = 0.0f;
        t = 0;
    }
    if(keyboard->getGate()){
        //std::cout << Trigger << std::endl;
        if(t <= attackEch){
            //std::cout << "A" << std::endl;
            ADSRcoef = (attackEch > 0) ? ADSRcoef + (1.0f/(float)attackEch) : 1.0f;
            t++;
            
        }else if(t > attackEch && t <= (attackEch+decayEch)){
            //std::cout << "D" << std::endl;
            ADSRcoef = (decayEch > 0 && ADSRcoef > sustain) ? ADSRcoef - 1.0f/(float)decayEch : sustain;
            t++;
        }else if(t > (attackEch+decayEch)){
            ADSRcoef = sustain;
            t++;
        }
    }else if(!keyboard->getGate()){
        ADSRcoef = (ADSRcoef > 0.0f && releaseEch > 0) ? ADSRcoef - (1.0f/(float)releaseEch) : 0.0f; 
        t = 0;
    }
}

float ADSR_envelop::getADSR_Coef(){
    //std::cout << t << "|" << attackEch << "|" << decayEch << std::endl;
    return(ADSRcoef);
}

void ADSR_envelop::setAttack(double coef){
    attackEch = (coef > 0.0f && coef <= 1.0f) ? coef*maxAttack*sampleRate : 0.0f;
    std::cout << "ADSR attack : " << (float)attackEch / (float)sampleRate << std::endl;
}

void ADSR_envelop::setDecay(double coef){
    decayEch = (coef > 0.0f && coef <= 1.0f) ? coef*maxDecay*sampleRate : 0.0f;
    std::cout << "ADSR decay : " << (float)decayEch / (float)sampleRate << std::endl;
}

void ADSR_envelop::setSustain(double coef){
    sustain = (coef > 0.0f && coef <= 1.0f) ? coef : 0.0f;
    std::cout << "AR sustain : " << sustain << std::endl;
}

void ADSR_envelop::setRelease(double coef){
    releaseEch = (coef > 0.0f && coef <= 1.0f) ? coef*maxRelease*sampleRate : 0.0f;
    std::cout << "ADSR release : " << (float)releaseEch / (float)sampleRate << std::endl;
}