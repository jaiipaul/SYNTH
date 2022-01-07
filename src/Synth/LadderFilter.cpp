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
    StageOutputs = (double*)calloc(4, sizeof(double));
    StageFeedbacks = (double*)calloc(4, sizeof(double));
    StageT = (double*)calloc(4, sizeof(double));
    for(int stage = 0; stage < 4; stage++){
        StageOutputs[stage] = 0.f;
        StageFeedbacks[stage] = 0.f;
    }
    ComputeCoef();
}

void LadderFilter::genOutput(PaData* Data){
    /* Copyright 2012 Stefano D'Angelo <zanga.mail@gmail.com> */ 
    double X = Data->left_phase + 4*resonance*StageOutputs[3];

    double Stage0 = -G*(tanh( X/(2*Vt)) + StageT[0]);
    StageOutputs[0] += (Stage0 + StageFeedbacks[0]) / (2.f * sampleRate);
    StageFeedbacks[0] = Stage0;
    StageT[0] = tanh(StageOutputs[0]/(2*Vt));

    double Stage1 = G*(StageT[0] - StageT[1]);
    StageOutputs[1] += (Stage1 + StageFeedbacks[1]) / (2.f * sampleRate);
    StageFeedbacks[1] = Stage1;
    StageT[1] = tanh(StageOutputs[1]/(2*Vt));

    double Stage2 = G*(StageT[1] - StageT[2]);
    StageOutputs[2] += (Stage2 + StageFeedbacks[2]) / (2.f * sampleRate);
    StageFeedbacks[2] = Stage2;
    StageT[2] = tanh(StageOutputs[2]/(2*Vt));

    double Stage3 = G*(StageT[2] - StageT[3]);
    StageOutputs[3] += (Stage3 + StageFeedbacks[3]) / (2.f * sampleRate);
    StageFeedbacks[3] = Stage3;
    StageT[3] = tanh(StageOutputs[3]/(2*Vt));

    // StageOutputs[0] = StageFeedbacks[0] + 2*Vt*G*(Tanh(X/(2*Vt)) - Tanh(StageFeedbacks[0]/(2*Vt)));
    // StageFeedbacks[0] = StageOutputs[0];

    // StageOutputs[1] = StageFeedbacks[1] + 2*Vt*G*(Tanh(StageOutputs[0]/(2*Vt)) - Tanh(StageFeedbacks[1]/(2*Vt)));
    // StageFeedbacks[1] = StageOutputs[1];
    
    // StageOutputs[2] = StageFeedbacks[2] + 2*Vt*G*(Tanh(StageOutputs[1]/(2*Vt)) - Tanh(StageFeedbacks[2]/(2*Vt)));
    // StageFeedbacks[2] = StageOutputs[2];
    
    // StageOutputs[3] = StageFeedbacks[3] + 2*Vt*G*(Tanh(StageOutputs[2]/(2*Vt)) - Tanh(StageFeedbacks[3]/(2*Vt)));
    // StageFeedbacks[3] = StageOutputs[3];

    

    Data->left_phase  = StageOutputs[3];
    Data->right_phase = StageOutputs[3];
    //std::cout <<StageOutputs[3] << std::endl;
}

void LadderFilter::ComputeCoef(){
    float x = (PI * cutoff)/sampleRate;
    G = 4.f * PI * Vt * cutoff * (1.f - x)/(1.f + x);
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