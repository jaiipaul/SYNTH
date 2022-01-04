#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_
#include <string>
#include "audioEngine.h"

class Keyboard;
class LFO;

class Oscillator{
private:
    int waveForm;
    float Frequency;
    float maxFreq = 2000.0;
    float minFreq = 15.0;
    unsigned long sampleRate;

    float PulseWidth;
    Keyboard* keyboard;

    LFO* oscLFO;
    bool oscLFO_linked;
    float freqLFO_intensity;
    float maxFreqMod = 5.f;
    float PwLFO_intensity;
    float maxPwMod   = 0.04f;

public:
    Oscillator();
    ~Oscillator();

    void init(int _waveForm, unsigned long _sampleRate);
    void init(int _waveForm, unsigned long _sampleRate, Keyboard* _keyboard);
    void generateWave(PaData* Data);

    void setLFO(LFO* _oscLFO);
    void setLFO_freq_intensity(double coef);
    void setLFO_Pw_intensity(double coef);
    void switchWaveForm();
    void setWaveForm(int form);
    void setFreq(double coef);
    void setPulseWidth(double coef);
};
#endif //_OSCILLATOR_H_