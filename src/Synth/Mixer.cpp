#include "Mixer.h"
#include "Oscillator.h"
#include <iostream>
Mixer::Mixer(){

}

Mixer::~Mixer(){

}

void Mixer::init(int _N_Osc){
    N_Osc = _N_Osc;
    //Oscs = _Oscillators;

    for(int i = 0; i < N_Osc; i++){
        volumes.push_back(0.f);
        std::cout << "set volume " << i << std::endl; 
        setVolume(i, 1.0);
    }
}

void Mixer::generateOutput(PaData* Data, std::vector<Oscillator*> Oscs){ 
    int v_idx = 0;
    for(auto o : Oscs){
        Data->left_phase   += volumes[v_idx]*o->generateWave();
        Data->right_phase += volumes[v_idx]*o->generateWave();
        v_idx++;
    }
    Data->left_phase = Data->left_phase/(float)N_Osc;
    Data->right_phase = Data->right_phase/(float)N_Osc;
}

void Mixer::setVolume(int VolumeIdx, double coef){
    volumes.at(VolumeIdx) = (float)coef;
}
