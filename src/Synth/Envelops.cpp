#include "Envelops.h"
#include <iostream>

// Attack-Release Envelop //
AR_envelop::AR_envelop(){

}

AR_envelop::~AR_envelop(){

}

void AR_envelop::init(unsigned long _sampleRate){
    sampleRate = _sampleRate;
    attackEch  = maxAttack*sampleRate;
    releaseEch = maxRelease*sampleRate;

    ARcoef = 0.0f;
}

void AR_envelop::Update(bool Gate, bool Trigger){
    if(Trigger) ARcoef = 0.0f;
    if(Gate){
        ARcoef = (ARcoef < 1.0f && attackEch > 0) ? ARcoef + 1.0f/(float)attackEch : 1.0f;
    }else if(!Gate){
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

void ADSR_envelop::init(unsigned long _sampleRate){
    sampleRate = _sampleRate;
    attackEch  = maxAttack*sampleRate;
    decayEch   = maxDecay*sampleRate;
    releaseEch = maxRelease*sampleRate;

    t = 0;
    ADSRcoef = 0.0f;
}

void ADSR_envelop::Update(bool Gate, bool Trigger){
    if(Trigger){
        ADSRcoef = 0.0f;
        t = 0;
    }
    if(Gate){
        if(t <= attackEch){
            ADSRcoef = (attackEch > 0) ? ADSRcoef + 1.0f/(float)attackEch : 1.0f;
            t++;
        }else if(t > attackEch && t <= (attackEch+decayEch)){
            ADSRcoef = (decayEch > 0 && ADSRcoef > sustain) ? ADSRcoef - 1.0f/(float)decayEch : sustain;
            t++; 
        }else{
            ADSRcoef = sustain;
        }
    }else if(!Gate){
        ADSRcoef = (ADSRcoef > 0.0f && releaseEch > 0) ? ADSRcoef - (1.0f/(float)releaseEch) : 0.0f; 
    }
}

float ADSR_envelop::getADSR_Coef(){
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