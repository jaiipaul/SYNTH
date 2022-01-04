#include "LadderFilter.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <complex>

float Tanh(float X);

LadderFilter::LadderFilter(){

}

LadderFilter::~LadderFilter(){

}

void LadderFilter::init(unsigned long _sampleRate){
    sampleRate = (float)_sampleRate;
    cutoff = maxCutoff;
    StageOutputs = (float*)calloc(4, sizeof(float));
    StageFeedbacks = (float*)calloc(4, sizeof(float));

    for(int stage = 0; stage < 4; stage++){
        StageOutputs[stage] = 0.f;
        StageFeedbacks[stage] = 0.f;
    }
    ComputeCoef();
}

void LadderFilter::genOutput(PaData* Data){

    float X = Data->left_phase - 4*resonance*StageFeedbacks[3];

    StageOutputs[0] = StageFeedbacks[0] + 2*Vt*G*(Tanh( X/(2*Vt)) - Tanh(StageFeedbacks[0]/(2*Vt)));
    StageOutputs[1] = StageFeedbacks[1] + 2*Vt*G*(Tanh(StageOutputs[0]/(2*Vt)) - Tanh(StageFeedbacks[1]/(2*Vt)));
    StageOutputs[2] = StageFeedbacks[2] + 2*Vt*G*(Tanh(StageOutputs[1]/(2*Vt)) - Tanh(StageFeedbacks[2]/(2*Vt)));
    StageOutputs[3] = StageFeedbacks[3] + 2*Vt*G*(Tanh(StageOutputs[2]/(2*Vt)) - Tanh(StageFeedbacks[3]/(2*Vt)));

    StageFeedbacks[0] = StageOutputs[0];
    StageFeedbacks[1] = StageOutputs[1];
    StageFeedbacks[2] = StageOutputs[2];
    StageFeedbacks[3] = StageOutputs[3];

    Data->left_phase  = StageOutputs[3];
    Data->right_phase = StageOutputs[3];
    //std::cout << Data->left_phase << std::endl;
}

void LadderFilter::ComputeCoef(){
    std::complex<float> w (0, (2.0f*PI*cutoff) / sampleRate);
    std::cout << abs(w) << std::endl;
    G = 1.0f - exp(-(2.0f*PI*cutoff) / sampleRate);
}

void LadderFilter::setCutoff(double coef){
    cutoff = (coef >= 0.f && coef <= 1.f) ? coef*maxCutoff : maxCutoff;
    ComputeCoef();
    std::cout << "Filter Cutoff : " << cutoff  << "("<<G<<")"<< std::endl;
}

void LadderFilter::setResonance(double coef){
    resonance = (coef >= 0.f && coef <= 1.f) ? (float)coef: 0.f;
    std::cout << "Filter Resonance : " << resonance << std::endl;
}

float Tanh(float X){
    return( (float)tanh(X) );
}