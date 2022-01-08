#ifndef _ENVELOPS_H_
#define _ENVELOPS_H_

#include "typeDef.h"

class Keyboard;

class AR_envelop{
private:
    int attackEch, releaseEch;

    float maxAttack = 5;
    float maxRelease = 5;

    unsigned long sampleRate;

    float ARcoef;
    Keyboard* keyboard;
    
public:
    AR_envelop();
    ~AR_envelop();

    void init(unsigned long _sampleRate, Keyboard* _keyboard);

    void Update();

    float getAR_Coef();

    void setAttack(double coef);
    void setRelease(double coef);
};

class ADSR_envelop{
private:
    int attackEch, decayEch, releaseEch;
    float sustain;

    float maxAttack  = 5;
    float maxDecay   = 5;
    float maxRelease = 5;

    unsigned long sampleRate;

    float ADSRcoef;
    int t;

    Keyboard* keyboard;

public:
    ADSR_envelop();
    ~ADSR_envelop();

    void init(unsigned long _sampleRate, Keyboard* _keyboard);

    void Update();

    float getADSR_Coef();

    void setAttack(double coef);
    void setDecay(double coef);
    void setSustain(double coef);
    void setRelease(double coef);
};

#endif // _ENVELOPS_H_