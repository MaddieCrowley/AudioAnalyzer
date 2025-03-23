#ifdef RTAUDIO_SUPPORT_ENABLE
#include "audio/rt.h"
#elifdef PULSEAUDIO_SUPPORT_ENABLE
#include "audio/pulse.h"
#endif

#ifdef SDL3
#define SDL_MAIN_HANDLED
#include "gui/sdl.h"
#elifdef X11
#include "gui/xwin.h"
#endif
#define SMALL_BUFFER 5
#define STRING(s) #s
#define TOSTRING(s) STRING(s)

std::string lr = "LR ";
std::string xy = "XY ";
std::string frame = " Frames";
std::string buff = TOSTRING(SMALL_BUFFER);
//TODO Work on README.md
// work on non stationary sub-buffer implementation
// add generic sdl gui class and generic window type class (maybe)
// add FFT with KISS_FFT
// add RtAudio file to 3rdParty folder
// continue on documentation
// add ability to change device
// add ability to change small buffer size (maybe) (would require updating dataSize gui class)
int main (int argc, char *argv[]) {
    audio a(SMALL_BUFFER);
    gui winLRS(gui::LR, 1920, 1440/3,
              "LR",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE);
    gui winLRM(gui::LR, 1920, 1440/3,
             (lr+buff+frame).c_str(),audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    gui winLong(gui::LR, 1920, 1440/3,
                "Full buffer",audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*BUFFER_SIZE,250);
    gui winXY(gui::XY, 1440/3, 1440/3,
             (xy+buff+frame).c_str(),audiodata::data,AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER);
    while (!winLRS.gDone&&!winLRM.gDone&&!winLong.gDone&&!winXY.gDone) {

#ifdef TIMING_DEBUG
        uint32_t start = SDL_GetTicks();
#endif
#ifdef PULSEAUDIO_SUPPORT_ENABLE
        a.read();
#endif
        winLRS.loop();
        winLRS.changeDataPtr(audiodata::data+(AUDIO_CHANNELS*AUDIO_SIZE*audiodata::frameNum));

#ifdef NON_STATIONARY_BUFFER
        //TODO work on this
        winLRM.changeDataPtr(audiodata::data+
                             (AUDIO_CHANNELS*AUDIO_SIZE*audiodata::frameNum)
                             *(audiodata::frameNum<(BUFFER_SIZE-SMALL_BUFFER)&&audiodata::frameNum>SMALL_BUFFER)
        );
#else

        /***
     *Branchless pointer change
     *current data which is appended to end of main buffer when frameNum == BUFFER_SIZE
     *TODO add option for SMALL_BUFFER sized view of previous frames (ifdef option probably)
     * shorten code below (probably add inline function to clean up)
     ***/
        winLRM.changeDataPtr(audiodata::data+
                             AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER*((audiodata::frameNum-SMALL_BUFFER)/SMALL_BUFFER)
                             *(audiodata::frameNum>SMALL_BUFFER)
                             +AUDIO_CHANNELS*AUDIO_SIZE*(BUFFER_SIZE-SMALL_BUFFER)
                             *(audiodata::frameNum<SMALL_BUFFER)
        );
        winXY.changeDataPtr(audiodata::data+
                            AUDIO_CHANNELS*AUDIO_SIZE*SMALL_BUFFER*((audiodata::frameNum-SMALL_BUFFER)/SMALL_BUFFER)
                            *(audiodata::frameNum>SMALL_BUFFER)
                            +AUDIO_CHANNELS*AUDIO_SIZE*(BUFFER_SIZE-SMALL_BUFFER)
                            *(audiodata::frameNum<SMALL_BUFFER)
        );
#endif
        winLRM.loop();
        winXY.loop();
        winLong.loop();
        SDL_Delay(7);
#ifdef TIMING_DEBUG
        uint32_t end = SDL_GetTicks();
        float delta = (float)(end-start);
        std::cout << float(delta) << "\n";
#endif
    }
    return 0;
}