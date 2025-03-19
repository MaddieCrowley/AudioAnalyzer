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
#define SMALL_BUFFER 10
audio a;
gui winXY(gui::XY, 400, 400,
          "XY 1 Frame",read::data,AUDIO_SIZE);
gui winL(gui::L, 1920, 600,
         "L 4 Frames",read::data,AUDIO_SIZE*SMALL_BUFFER);
gui winR(gui::R, 1920, 600,
         "R 4 Frames",read::data,AUDIO_SIZE*SMALL_BUFFER);
gui winLong(gui::XY, 600, 600,
            "Full buffer",read::data,AUDIO_SIZE*BUFFER_SIZE);
int main (int argc, char *argv[]) {
    while (!winXY.gDone) {
        winXY.loop();
        winXY.changeDataPtr(read::data+(AUDIO_SIZE*read::frameNum));
        winL.loop();
        winR.loop();
        if (read::frameNum%(SMALL_BUFFER)==0&&read::frameNum!=0) {
            //std::cout << read::frameNum << std::endl;
            winL.changeDataPtr(read::data+(AUDIO_SIZE*(read::frameNum-SMALL_BUFFER)));
            winR.changeDataPtr(read::data+(AUDIO_SIZE*(read::frameNum-SMALL_BUFFER)));
        }
        winLong.loop();
    }
    return 0;
}