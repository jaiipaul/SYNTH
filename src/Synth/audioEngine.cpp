#include "audioEngine.h"
#include "Oscillator.h"
#include <iostream>

AudioEngine::AudioEngine(){
}

AudioEngine::~AudioEngine(){
}

void AudioEngine::init(){
    sampleRate = 44100;
    framePerBuffer = 256;

    Data = (PaData*)calloc(1, sizeof(PaData));
    Data->t = 0;
    StartEngine();
}

void AudioEngine::init(unsigned long _sampleRate, unsigned long _bufferSize){
    sampleRate = _sampleRate;
    framePerBuffer = _bufferSize;

    Data = (PaData*)calloc(1, sizeof(PaData));
    Data->t = 0;
    StartEngine();
}

void AudioEngine::clean(){
    if(isStreamOpened()){
        CloseStream();
    }
    StopEngine();
}
//void AudioEngine::SetOutputGen(void (*func)(PaData*)){
//    OutputFunc = func;
//}
//
//void AudioEngine::GenOutput(PaData* Data){
//    OutputFunc(Data);
//}

void AudioEngine::SetCallBackFunc(CallBack _callBackFunc){
    callbackFunc = _callBackFunc;
}

// int AudioEngine::Callback( const void *inputBuffer, void *outputBuffer,
//                            unsigned long framesPerBuffer,
//                            const PaStreamCallbackTimeInfo* timeInfo,
//                            PaStreamCallbackFlags statusFlags,
//                            void *userData )
// {       
//     /* Cast data passed through stream to our structure. */
//     PaData *callbackData = (PaData*)userData; 
//     float *out = (float*)outputBuffer;
//     unsigned int i;
//     (void) inputBuffer; /* Prevent unused variable warning. */
    
//     for( i=0; i<framesPerBuffer; i++ )
//     {    
//         GenOutput(callbackData);
//         out[2*i]   = callbackData->left_phase;  /* left */
//         out[2*i+1] = callbackData->right_phase; /* right */
//         // /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
//         // callbackData->left_phase += 0.01f;
//         // /* When signal reaches top, drop back down. */
//         // if( callbackData->left_phase >= 1.0f ) callbackData->left_phase -= 2.0f;
//         // /* higher pitch so we can distinguish left and right. */
//         // callbackData->right_phase += 0.03f;
//         // if( callbackData->right_phase >= 1.0f ) callbackData->right_phase -= 2.0f;
//     }
//     return 0;
// }

void AudioEngine::StartEngine(){
    err = Pa_Initialize();
    streamOpened  = false;
    streamStarted = false;
    Error();
}

void AudioEngine::StopEngine(){
    err = Pa_Terminate();
    Error();
}
void AudioEngine::ListDevices(){
    PaDeviceIndex nDevices = Pa_GetDeviceCount();
    std::cout << "## PortAudio : " << nDevices << " devices available" << std::endl;
    
    for(int device = 0; device < nDevices; device++){
        std::cout << "<" << device << "> - " << Pa_GetDeviceInfo(device)->name << std::endl;	
    }
}

void AudioEngine::OpenDefaultStream(){
    PaStreamParameters outputParameters;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream( &stream,
                        NULL,          /* no input channels */
                        &outputParameters,          /* stereo output */ 
                        sampleRate,
                        framePerBuffer,
                        paDitherOff,
                        callbackFunc,
                        Data);
    Error();
    if(err == paNoError){
        streamOpened = true;
    }
    std::cout << "stream opened" << std::endl;
}

void AudioEngine::CloseStream(){
    err = Pa_CloseStream( stream );
    Error();
    if(err == paNoError){
        streamOpened = false;
    }
}

void AudioEngine::StartStream(){
    err = Pa_StartStream( stream );
    if(err == paNoError) streamStarted = true;
    Error();
}

void AudioEngine::StopStream(){
    err = Pa_StopStream( stream );
    if(err == paNoError) streamStarted = false;
    Error();
}


bool AudioEngine::isStreamOpened(){
    return(streamOpened);
}


void AudioEngine::Error(){
    if( err != paNoError )
        std::cout << "PortAudio error: " << Pa_GetErrorText( err ) << std::endl;
}