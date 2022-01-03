#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_
#include <string>
#include "audioEngine.h"

class Keyboard;

class Oscillator{
private:
    std::string type;
    float Frequency;
    float maxFreq = 2000.0;
    float minFreq = 15.0;
    unsigned long sampleRate;

    Keyboard* keyboard;

public:
    Oscillator();
    ~Oscillator();

    void init(std::string _type, unsigned long _sampleRate);
    void init(std::string _type, unsigned long _sampleRate, Keyboard* _keyboard);
    void generateWave(PaData* Data);
    void setFreq(double coef);
};
#endif //_OSCILLATOR_H_