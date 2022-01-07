#ifndef _OSCILLATOR_H_
#define _OSCILLATOR_H_
#include <string>
#include "audioEngine.h"

class Keyboard;
class LFO;

class Oscillator{
private:
    int waveForm;
    int t;
    float time;
    int   octave;
    float baseFrequency;
    float Frequency;
    float maxFreq = 2000.0;
    float minFreq = 15.0;
    float sampleRate;

    float PulseWidth;
    float PulseWidthMod;
    Keyboard* keyboard;

    LFO* oscLFO;
    bool oscLFO_linked;
    float freqLFO_intensity;
    float maxFreqMod = 1.f;
    float PwLFO_intensity;
    float maxPwMod   = 0.04f;

    bool sync;
    float detune;

public:
    Oscillator();
    ~Oscillator();

    void init(int _waveForm, int _octave, unsigned long _sampleRate);
    void init(int _waveForm, int _octave, unsigned long _sampleRate, Keyboard* _keyboard);
    float generateWave();

    void setLFO(LFO* _oscLFO);
    void setLFO_freq_intensity(double coef);
    void setLFO_Pw_intensity(double coef);
    void switchWaveForm();
    void setWaveForm(int form);
    void setOctave(int oct);
    void setBaseFreq(double coef);
    void setPulseWidth(double coef);
    void setDetune(double coef);
};
#endif //_OSCILLATOR_H_