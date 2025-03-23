//
// Created by Maddie on 3/17/2025.
//

#ifndef RT_H
#define RT_H
#include "../DEFINE.h"
#include <rtaudio/RtAudio.h>
class audio
{
  RtAudio adc;
  RtAudio::StreamParameters params;
public:
    explicit audio(int initFrame);
    ~audio();
};
namespace audiodata
{
    inline int frameNum;
    int read(void*outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                  double streamTime,RtAudioStreamStatus status,void*userData);
    inline int16_t data[AUDIO_SIZE*(BUFFER_SIZE+1)*AUDIO_CHANNELS];
}
#endif //RT_H
