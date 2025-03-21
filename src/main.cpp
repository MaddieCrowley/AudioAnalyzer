#ifdef RTAUDIO_SUPPORT_ENABLE
#include "audio/rt.h"
//#define a.audiodata() audiodata::audiodata()

#elifdef PULSEAUDIO_SUPPORT_ENABLE
#include "audio/pulse.h"
#endif

#ifdef SDL3
#define SDL_MAIN_HANDLED
#include "gui/sdl.h"
#elifdef X11
#include "gui/xwin.h"
#endif
#define SMALL_BUFFER 10

//TODO parallel audio sample taking and window drawing to minimize the number of audio frames dropped

int main (int argc, char *argv[]) {
#ifdef CORRECT_SHORT_BUFFER_WRAP
    int16_t tempData[SMALL_BUFFER*AUDIO_SIZE*AUDIO_CHANNELS];
#endif
    audio a;
    gui winLRS(gui::LR, 1920, 1440/3,
              "LR",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE);
    gui winLRM(gui::LR, 1920, 1440/3,
             "LR 10 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    // gui winR(gui::R, 1920, 1440/3,
    //          "R 4 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winLong(gui::All, 1920, 1440/3,
    "Full buffer",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*BUFFER_SIZE,10);
    gui winXY(gui::XY, 1440/3, 1440/3,
             "XY 10 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    while (!winLRS.gDone&&!winLRM.gDone&&!winXY.gDone&&!winLong.gDone) {

#ifdef TIMING_DEBUG
        uint32_t start = SDL_GetTicks();
#endif
#ifdef PULSEAUDIO_SUPPORT_ENABLE
        a.read();
#endif
        winLRS.loop();
        winLRM.loop();
        winXY.loop();
        winLRS.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*audiodata::frameNum));
        //BUG Still some glitching, most visible when going from playing to pause
        winLRM.changeDataPtr(audiodata::data+AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum/SMALL_BUFFER)*SMALL_BUFFER);
        winXY.changeDataPtr(audiodata::data+SMALL_BUFFER*AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum/SMALL_BUFFER));
        winLong.loop();
#ifdef TIMING_DEBUG
        uint32_t end = SDL_GetTicks();
        float delta = (float)(end-start);
        std::cout << float(delta) << "\n";

#endif
    }
    return 0;
}