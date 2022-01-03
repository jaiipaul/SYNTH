#include "Filter.h"
#include <stdlib.h>
#include <iostream>

Filter::Filter(){

}

Filter::~Filter(){

}

void Filter::init(unsigned long sampleRate){
    sampleTime = 1/(float)sampleRate;
    cutoff = maxCutoff;
    feedbackY = 0.0f;
    feedbackX = 0.0f;

    coefficients = (float*)calloc(2, sizeof(float));
    ComputeCoefs();
}

void Filter::genOutput(PaData* Data){
    //float feedbackX1 = feedbackX;
    //feedbackX  = Data->left_phase;

    //Data->left_phase  = coefficients[0]*feedbackY + coefficients[1]*(Data->left_phase  + feedbackX1);
    //Data->right_phase = coefficients[0]*feedbackY + coefficients[1]*(Data->right_phase + feedbackX1);

    Data->left_phase  = coefficients[0]*Data->left_phase + (1.0f - coefficients[0])*feedbackY;
    Data->right_phase = coefficients[0]*Data->left_phase + (1.0f - coefficients[0])*feedbackY;
    
    feedbackY = Data->left_phase;
}

void Filter::ComputeCoefs(){
    //coefficients[0] = (1.0f - 2*(2*PI)/(cutoff*sampleTime)) /  (1.0f + 2*(2*PI)/(cutoff*sampleTime));
    coefficients[1] =  1.0f;//  /  (1.0f + 2*(2*PI)/(cutoff*sampleTime));
    coefficients[0] = sampleTime /( (1 / (2*PI*cutoff)) + sampleTime );
    std::cout << coefficients[0] << " | " << coefficients[1] << std::endl;
}

void Filter::setCutoff(double coef){
    cutoff = (coef >= 0.f && coef <= 1.f) ? minCutoff + coef*(maxCutoff - minCutoff) : maxCutoff;
    std::cout << "Filter Cutoff : " << cutoff << std::endl;
    ComputeCoefs();
}