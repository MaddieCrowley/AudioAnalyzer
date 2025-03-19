#ifndef DEFINE_H
#define DEFINE_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#define AUDIO_SAMPLE_FORMAT S16_LE
#define AUDIO_SIZE 1024
#ifdef LOW_POWER_MODE
#define AUDIO_SAMPLE_RATE 48000
#else
#define AUDIO_SAMPLE_RATE 192000
#endif
#define AUDIO_CHANNELS 2
#define BUFFER_SIZE 100
#endif //DEFINE_H