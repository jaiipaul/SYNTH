#ifndef _SYNTH_CONTROLS_H_
#define _SYNTH_CONTROLS_H_

/* CONTROL FUNCTIONS */
// AUDIO ENGINE
void StartStream();
void StopStream();
void ListDevices();

// OSCS
// OSC 1 controls
void SwitchOSC1WaveForm();
void SetPulseWidth1(double coef);
void SetFreqMod1(double coef);
void SetPwMod1(double coef);


// OSC 2 controls
void SwitchOSC2WaveForm();
void SetPulseWidth2(double coef);
void SetFreqMod2(double coef);
void SetPwMod2(double coef);
void SetDetune2(double coef);

// MIXER controls
void SetVolumeSub(double coef);
void SetVolume1(double coef);
void SetVolume2(double coef);

// VCA controls
void SetGlobalVolume(double coef);
void SetVCAEnvLvl(double coef);
void SwitchEnvTypeVCA();

// ENVELOPS controls
void SetAR_Attack1(double coef);
void SetAR_Release1(double coef);
void SetADSR_Attack1(double coef);
void SetADSR_Decay1(double coef);
void SetADSR_Sustain1(double coef);
void SetADSR_Release1(double coef);

void SetAR_Attack2(double coef);
void SetAR_Release2(double coef);
void SetADSR_Attack2(double coef);
void SetADSR_Decay2(double coef);
void SetADSR_Sustain2(double coef);
void SetADSR_Release2(double coef);

// FILTER controls
void SetCutoff(double coef);
void SetResonance(double coef);
void SetDrive(double coef);
void SwitchEnvTypeVCF();
void SetVCFEnvLvl(double coef);
void SetVCF_LFOint(double coef);


// LFOs controls
void SetLFO1Rate(double coef);
void SetLFO2Rate(double coef);
void SwitchLFO1WaveForm();
void SwitchLFO2WaveForm();


#endif // _SYNTH_CONTROLS_H_