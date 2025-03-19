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
gui winXY(gui::L, 400, 400,
          "L Fast",read::data,AUDIO_SIZE);
gui winL(gui::L, 800, 600,
    "L medium",read::data,AUDIO_SIZE*BUFFER_SIZE/4);
gui winLong(gui::L, 800, 600,
    "L slow",read::data,AUDIO_SIZE*BUFFER_SIZE);
int main (int argc, char *argv[]) {
    while (!winXY.gDone) {
        winXY.loop();
        winXY.changeDataPtr(read::data+(AUDIO_SIZE*read::frameNum));
        winL.loop();
        winL.changeDataPtr(read::data+(AUDIO_SIZE*read::frameNum/4));//TODO weird behavior (due to not finishing going through points before buffer updates?)
        winLong.loop();
    }
    return 0;
}