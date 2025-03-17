#ifndef AUDIO_H
#define AUDIO_H

#ifdef PULSEAUDIO
class audio
{
  public:
    audio();
    ~audio();
    void read(int_16t*data);
}
#elif defined(RTAUDIO)
class audio
{
public:
  audio();
  ~audio();

}
namespace read
{
read();
}

#endif

#endif //AUDIO_H