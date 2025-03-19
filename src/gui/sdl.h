//
// Created by Maddie on 3/17/2025.
//

#ifndef SDL_H
#define SDL_H
#include "../DEFINE.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class gui {
public:
    enum winType{All,XY,L,R};
    explicit gui(winType windowT = All, int width = 1920, int height = 1080,
        const char* title="Audio Analyzer", int16_t*data = nullptr,int dataSize=AUDIO_SIZE,int stepSize=1);
    ~gui();
    void changeDataPtr(int16_t*data);
    void loop();
    int gDone;

private:
    bool update() const;
    void drawXY(SDL_Rect&,SDL_Point&);//Relies on the fact that the data buffer is a static memory location
    void drawLineL(SDL_Rect&,SDL_Point&);
    void drawLineR(SDL_Rect&,SDL_Point&);
    void render();
    struct guiRect{SDL_Rect rect;SDL_Point start;};

    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    winType windowType;
    const char* title;
    const int m_stepSize;

    static float scale(int x,int start,int length,int max);

    int16_t * data;
    int dataSize;

    int *gFrameBuffer;
    SDL_Window *gSDLWindow;
    SDL_Renderer *gSDLRenderer;
    SDL_Texture *gSDLTexture;

    guiRect renLeft{},renRight{},renXY{},WINDOW{};

};
#endif //SDL_H
