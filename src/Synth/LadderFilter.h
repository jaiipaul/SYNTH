#ifndef _LADDER_FILTER_H_
#define _LADDER_FILTER_H_

#include "../typeDef.h"

class AR_envelop;
class ADSR_envelop;
class LFO;

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
    float Drive;

    float sampleRate;

    bool AR_linked;
    bool ADSR_linked;
    int  ENV_type;
    float ENV_intensity;
    
    bool LFO_linked;
    float maxCutoffMod = 5000.f;
    float LFO_intensity;

public:
    AR_envelop*   AR;
    ADSR_envelop* ADSR;
    LFO* Lfo;
    LadderFilter();
    ~LadderFilter();

    void init(unsigned long sampleRate);
    void genOutput(PaData* Data);

    void ComputeCoef();
    void setCutoff(double coef);
    void setResonance(double coef);
    void setDrive(double coef);

    void bindLFO(LFO* _Lfo);
    void setLFOintensity(double coef);
    void bindAR(AR_envelop* _AR);
    void bindADSR(ADSR_envelop* _ADSR);
    void setENVintensity(double coef);
};
#endif // _FILTER_H_
