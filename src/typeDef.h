#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <portaudio.h>

typedef void (*buttonFunc)();
typedef void (*sliderFunc)(double);
typedef float (*signalGen)();

typedef int (*CallBack)(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

typedef struct
{   
    float left_phase;
    float right_phase;
    int t;
}   
PaData;

typedef enum{
    SQR, SAW, TRI, SINE

}WaveType;

typedef enum{
    GATE, AR, ADSR
}EnvelopType;

#define PI 3.141592653589793
#define Vt 0.312


#endif // _TYPEDEF_H_