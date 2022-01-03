#ifndef _FILTER_H_
#define _FILTER_H_

#include "typeDef.h"

class Filter{
private :
    float cutoff;
    float maxCutoff = 10000.0f;
    float minCutoff = 15.0f;

    float feedbackY;
    float feedbackX;
    float* coefficients;

    float sampleTime;
public:
    Filter();
    ~Filter();

    void init(unsigned long sampleRate);
    void genOutput(PaData* Data);
    void Update();
    void ComputeCoefs();
    void setCutoff(double coef);
};
#endif // _FILTER_H_