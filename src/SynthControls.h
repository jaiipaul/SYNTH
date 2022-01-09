#ifndef _SYNTH_CONTROLS_H_
#define _SYNTH_CONTROLS_H_

/* CONTROL FUNCTIONS */
void StartStream();
void StopStream();
void ListDevices();
void SetPulseWidth(double coef);
void SetVolume(double coef);
void SetEnvLvl(double coef);
void SetAR_Attack(double coef);
void SetAR_Release(double coef);
void SetADSR_Attack(double coef);
void SetADSR_Decay(double coef);
void SetADSR_Sustain(double coef);
void SetADSR_Release(double coef);
void SwitchEnvType();
void SetCutoff(double coef);
void SetResonance(double coef);
void SwitchWaveForm();
void SetFreqMod(double coef);
void SetPwMod(double coef);
void SetLfoRate(double coef);
void SetDetune(double coef);

#endif // _SYNTH_CONTROLS_H_