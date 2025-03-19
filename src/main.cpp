#ifdef RTAUDIO_SUPPORT_ENABLE
#include "audio/rt.h"
//#define a.audiodata() audiodata::audiodata()

#elifdef PULSEAUDIO_SUPPORT_ENABLE
#include "audio/pulse.h"
#endif

#ifdef SDL3
#include "gui/sdl.h"
#elifdef X11
#include "gui/xwin.h"
#endif
#define SMALL_BUFFER 4
#include <iostream>

//TODO parallel audio sample taking and window drawing to minimize the number of audio frames dropped

int main (int argc, char *argv[]) {
    audio a;
    gui winXY(gui::L, 1920, 600,
              "L 1/2 Frame",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE/2);
    gui winL(gui::L, 1920, 600,
             "L 4 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winR(gui::R, 1920, 600,
             "R 4 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winLong(gui::All, 1920, 1080,
                "Full buffer",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*BUFFER_SIZE);
    while (!winL.gDone) {
        #ifdef PULSEAUDIO_SUPPORT_ENABLE
        a.read();
        #endif
        winXY.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*audiodata::frameNum));
        winXY.loop();
        winL.loop(); //BUG TODO figure out why it stutters
        winR.loop();
        if (audiodata::frameNum%(SMALL_BUFFER)==0) {
            if (audiodata::frameNum==0) {
                winL.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*(BUFFER_SIZE-SMALL_BUFFER)));
                winR.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*(BUFFER_SIZE-SMALL_BUFFER)));
            }
            else {
                winL.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum-SMALL_BUFFER)));
                winR.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum-SMALL_BUFFER)));
            }
        }
        winLong.loop();
    }
    return 0;
}