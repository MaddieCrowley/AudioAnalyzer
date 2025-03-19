//
// Created by Maddie on 3/17/2025.
//

#ifndef PULSE_H
#define PULSE_H
#include <pulse/simple.h>
#include "../DEFINE.h"

class audio
{
public:
    audio();
    ~audio();
    int read();
private:
    pa_simple *s;
    pa_sample_spec ss;
};
namespace audiodata
{
inline int frameNum;
inline int16_t data[AUDIO_SIZE*BUFFER_SIZE];
}
#endif //PULSE_H
