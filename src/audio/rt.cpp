#include "rt.h"
#define uint unsigned int
audio::audio()
{
    audiodata::frameNum=0;
    params.deviceId = adc.getDefaultOutputDevice();
    params.nChannels = AUDIO_CHANNELS;
    params.firstChannel = 0;
    uint sampleRate = AUDIO_SAMPLE_RATE;
    uint bufferFrames = AUDIO_SIZE;
    adc.openStream(NULL,&params, RTAUDIO_SINT16,
                   sampleRate,&bufferFrames,&audiodata::read);
    adc.startStream();
}

audio::~audio() {
    adc.stopStream();
    adc.closeStream();
}

int audiodata::read(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
               double streamTime, RtAudioStreamStatus status, void *userData) {
    frameNum++;
    if (frameNum == BUFFER_SIZE) {
        frameNum = 0;
    }
    memcpy(data+frameNum*AUDIO_CHANNELS*AUDIO_SIZE,inputBuffer,AUDIO_CHANNELS*AUDIO_SIZE*sizeof(int16_t));
    return 0;
}