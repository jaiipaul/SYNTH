#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "portaudio.h"
#include "typeDef.h"
//float maxFreq = 0.1f;
//float minFreq = 0.0f;

class Synth;

class AudioEngine{
private:

    bool streamOpened;
    bool streamStarted;

    unsigned long sampleRate;
    unsigned long framePerBuffer;

    

public:
    CallBack callbackFunc;
    PaData* Data;
    PaError err; 
    PaStream* stream;

    //static void (*OutputFunc)(PaData*);
    //void SetOutputGen(void (*func)(PaData*));
    //static void GenOutput(PaData* Dtata);

    AudioEngine();
    ~AudioEngine();

    void init();
    void init(unsigned long _sampleRate, unsigned long _bufferSize);

    void clean();
    
    void StartEngine();
    void StopEngine();

    void ListDevices();

    void OpenDefaultStream();
    void OpenSteam();
    void CloseStream();
    void StartStream();
    void StopStream();

    bool isStreamOpened();

    void SetCallBackFunc(CallBack _callBackFunc);
    // static int Callback( const void *inputBuffer, void *outputBuffer,
    //                      unsigned long framesPerBuffer,
    //                      const PaStreamCallbackTimeInfo* timeInfo,
    //                      PaStreamCallbackFlags statusFlags,
    //                      void *userData );
    
    void Error();
};

#endif
