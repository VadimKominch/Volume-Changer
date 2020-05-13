#include <iostream>
#include "RtAudio.h"
#include <string.h>
#include <conio.h>

int signal_scale = 0;

// Pass-through function.
int scale(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* userData)
{
    // Note: do nothing here for pass through.
    double* input = reinterpret_cast<double*>(inputBuffer);
    double* output = reinterpret_cast<double*>(outputBuffer);

    //4701
    // process channel 

    //concat two channels together
    for (size_t i = 0; i < 512; ++i)
    {
        output[i] = input[i] * signal_scale/10;
    }
    return 0;
}

void changeScale()
{
    int input = 1;
    while (input != 13)
    {
        input = _getch();
        switch (input)
        {

        case 72:
            signal_scale = signal_scale < 10 ? signal_scale + 1 : signal_scale;
            break;
        case 80:
            signal_scale = signal_scale > 0 ? signal_scale - 1 : signal_scale;
            break;
        }
    }
}

int main()
{
    RtAudio audio;
    RtAudio::StreamParameters iParameters, oParameters;
    iParameters.deviceId = audio.getDefaultInputDevice();
    iParameters.nChannels = 2;
    oParameters.deviceId = audio.getDefaultOutputDevice();
    oParameters.nChannels = 2;
    unsigned int sampleRate = 44100;
    unsigned int data = 0, bufferFrames = 256;
    
    try {
        audio.openStream(&oParameters, &iParameters, RTAUDIO_FLOAT64,
            sampleRate, &bufferFrames, &scale, (void*)&data);
        data = bufferFrames * oParameters.nChannels * sizeof(RTAUDIO_FLOAT64);
        // Start the stream
        audio.startStream();
    }
    catch (RtAudioError & error) {
        error.printMessage();
    }

    std::cout << "\nRunning duplex ... press <enter> to quit.\n";
    //std::cin.get();
    changeScale();

    try {
        // Stop and close the stream
        audio.stopStream();
        if (audio.isStreamOpen()) audio.closeStream();
    }
    catch (RtAudioError & error) {
        error.printMessage();
    }


    return 0;
}