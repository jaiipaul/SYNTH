#ifndef _LADDER_FILTER_H_
#define _LADDER_FILTER_H_

#include "../typeDef.h"

class LadderFilter{
private :
    float cutoff;
    float maxCutoff = 20000.0f;
    float minCutoff = 0.0f;

    float resonance;

    double* StageOutputs;
    double* StageFeedbacks;
    double* StageT;

    float G;
    float comp;

    float sampleRate;
public:
    LadderFilter();
    ~LadderFilter();

    void init(unsigned long sampleRate);
    void genOutput(PaData* Data);
    void Update();
    void ComputeCoef();
    void setCutoff(double coef);
    void setResonance(double coef);
};
#endif // _FILTER_H_