#ifdef RTAUDIO_SUPPORT_ENABLE
#include "audio/rt.h"
//#define a.read() read::read()

#elifdef PULSEAUDIO_SUPPORT_ENABLE
#include "audio/pulse.h"
#endif

#ifdef SDL3
#include "gui/sdl.h"
#elifdef X11
#include "gui/xwin.h"
#endif

audio a;

int main (int argc, char *argv[]) {
}