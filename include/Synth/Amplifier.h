#ifndef _AMPLIFIER_H_
#define _AMPLIFIER_H_
#include "typeDef.h"
#include <string>

class Keyboard;
class AR_envelop;
class ADSR_envelop;

class Amplifier{
private:
    Keyboard* keyboard;

    float volume;
    int EnvType;
    float envelop_level;

public:
    AR_envelop* AR; 
    ADSR_envelop* ADSR;

    Amplifier();
    ~Amplifier();

    void init(unsigned long sampleRate, Keyboard* _keyboard);

    void genOutput(PaData* Data);
    void setVolume(double coef);
    void setEnvelopLevel(double coef);
    void switchEnvelopType();
    void setEnvelopType(int type);
};

#endif //_AMPLIFIER_H_