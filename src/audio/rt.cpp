#include "rt.h"
#define uint unsigned int
audio::audio(int initFrame) //Constructor for starting audio stream
{
    audiodata::frameNum=initFrame;
    params.deviceId = adc.getDefaultOutputDevice();
    params.nChannels = AUDIO_CHANNELS;
    params.firstChannel = 0;
    uint sampleRate = AUDIO_SAMPLE_RATE;
    uint bufferFrames = AUDIO_SIZE;
    adc.openStream(NULL,&params, RTAUDIO_SINT16,
                   sampleRate,&bufferFrames,&audiodata::read);
    adc.startStream();
}

audio::~audio() { //Destructor which terminates audio stream and frees up the buffer
    adc.stopStream();
    adc.closeStream();
}

int audiodata::read(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
               double streamTime, RtAudioStreamStatus status, void *userData) {
    //Read function that gets called whenever the buffer is full
    frameNum++;
    if (frameNum == BUFFER_SIZE) { //Appends data on the end for ease of wrap around with sub-buffers and resets frameNum
        memcpy(data+BUFFER_SIZE*AUDIO_SIZE*AUDIO_CHANNELS,inputBuffer,AUDIO_SIZE*AUDIO_CHANNELS*sizeof(int16_t));
        frameNum = 0;
    }
    memcpy(data+frameNum*AUDIO_CHANNELS*AUDIO_SIZE,inputBuffer,AUDIO_CHANNELS*AUDIO_SIZE*sizeof(int16_t));

    return 0;
}