#ifndef _LFO_H_
#define _LFO_H_
#include <string>
#include "audioEngine.h"

class Keyboard;

class LFO{
private:
    int waveForm;
    float Frequency;
    float maxFreq = 5.0;
    float minFreq = 0.0;
    float sampleRate;

    int   t;
    float time;
    float value;
    bool keySync;
    Keyboard* keyboard;

public:
    LFO();
    ~LFO();

    void init(int _waveForm, unsigned long _sampleRate);
    void init(int _waveForm, unsigned long _sampleRate, Keyboard* _keyboard);
    void generateWave();
    float getValue();
    
    void switchWaveForm();
    void setWaveForm(int form);
    void setFreq(double coef);
};
#endif //_LFO_H_