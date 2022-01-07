#ifndef _MIXER_H_
#define _MIXER_H_

#include <vector>
#include "../typeDef.h"

class Oscillator;

class Mixer{
private:
    int N_Osc;
    //std::vector<Oscillator*> Oscs;
    std::vector<float> volumes;

public:
    Mixer();
    ~Mixer();

    void init(int _N_Sig);
    void generateOutput(PaData* Data, std::vector<Oscillator*> Oscs);
    void setVolume(int VolumeIdx, double coef);
};

#endif _MIXER_H_