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

//TODO parallel audio sample taking and window drawing to minimize the number of audio frames dropped

int main (int argc, char *argv[]) {
#ifdef CORRECT_SHORT_BUFFER_WRAP
    int16_t tempData[SMALL_BUFFER*AUDIO_SIZE*AUDIO_CHANNELS];
#endif
    audio a;
    gui winXY(gui::L, 1920, 600,
              "L 1/2 Frame",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE);
    gui winL(gui::L, 1920, 600,
             "L 4 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winR(gui::R, 1920, 600,
             "R 4 Frames",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winLong(gui::All, 1920, 1080,
    "Full buffer",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*BUFFER_SIZE,10);
    while (!winL.gDone) {
#ifdef TIMING_DEBUG
        uint32_t start = SDL_GetTicks();
#endif
#ifdef PULSEAUDIO_SUPPORT_ENABLE
        a.read();
#endif
        winXY.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*audiodata::frameNum));
        winXY.loop();
        winL.loop();
        winR.loop();
//         if (audiodata::frameNum==0) {
//             //ONE OPTION BELOW (CORRECT BUT NOT AS PRETTY)
//             //Uses a temp variable to place last 3 frames of data and first frame of data (which has just been written in)
//             // But is slower and doesn't look as clean
// #ifdef CORRECT_SHORT_BUFFER_WRAP
//             memcpy(&tempData[SMALL_BUFFER*AUDIO_SIZE*AUDIO_CHANNELS-1]-(SMALL_BUFFER-1)*AUDIO_SIZE*AUDIO_CHANNELS,
//             &audiodata::data[BUFFER_SIZE*AUDIO_SIZE*AUDIO_CHANNELS-1]-(SMALL_BUFFER-1)*AUDIO_SIZE*AUDIO_CHANNELS,
//                (SMALL_BUFFER-1)*AUDIO_SIZE*AUDIO_CHANNELS*sizeof(int16_t));
//             memcpy(&tempData[0], &audiodata::data[0], AUDIO_SIZE*AUDIO_CHANNELS*sizeof(int16_t));
//             winL.changeDataPtr(tempData);
// #else
//             //OTHER OPTION (prettier, but not as correct)
//             //just uses the first 4 values of data
//             winL.changeDataPtr(audiodata::data);
//             winR.changeDataPtr(audiodata::data);
// #endif
//         }
//         else {
//         }
            winL.changeDataPtr(audiodata::data+SMALL_BUFFER*(AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum/SMALL_BUFFER)));
            winR.changeDataPtr(audiodata::data+SMALL_BUFFER*(AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum/SMALL_BUFFER)));
        // else if (audiodata::frameNum%SMALL_BUFFER==0) {
        //     winL.changeDataPtr(audiodata::data+AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum-SMALL_BUFFER));
        //     winR.changeDataPtr(audiodata::data+AUDIO_CHANNELS*AUDIO_SIZE*(audiodata::frameNum-SMALL_BUFFER));
        // }
        winLong.loop();
#ifdef TIMING_DEBUG
        uint32_t end = SDL_GetTicks();
        float delta = (float)(end-start);
        std::cout << float(delta) << "\n";

#endif
    }
    return 0;
}