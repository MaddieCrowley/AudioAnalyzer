#include "pulse.h"
//TODO add documentation
audio::audio()
{
    ss.format = PA_SAMPLE_S16NE;
    ss.channels = AUDIO_CHANNELS;
    ss.rate = AUDIO_SAMPLE_RATE;

    pa_buffer_attr buffer_attr;
    buffer_attr.maxlength = 2*AUDIO_SIZE;
    //FIXES problem with latency issues, causes discontinuities but *shrug*
    //(can low pass filter that data anyway b/c only for low frequencies on fft)
    buffer_attr.fragsize = (uint32_t) -1;

    s=pa_simple_new(NULL, "test", PA_STREAM_RECORD, NULL, "Music",
                    &ss, NULL, &buffer_attr, NULL);
}

audio::~audio() {
    pa_simple_free(s);
}

int audio::read() {
    audiodata::frameNum++;
    if (audiodata::frameNum == BUFFER_SIZE) {audiodata::frameNum = 0;}
    pa_simple_read(s, audiodata::data+audiodata::frameNum*AUDIO_SIZE, AUDIO_SIZE*sizeof(int16_t), NULL);
    return 1;
}
