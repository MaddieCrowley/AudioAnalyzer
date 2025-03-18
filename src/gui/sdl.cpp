#include "sdl.h"


gui::gui(winType windowT, int width, int height, const char* title, int16_t*data,int dataSize)
    : WINDOW_WIDTH(width),WINDOW_HEIGHT(height),windowType(windowT),
      title(title),data(data),dataSize(dataSize){

    gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    gSDLWindow = SDL_CreateWindow(title, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer,
                                    SDL_PIXELFORMAT_ABGR8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    WINDOW_WIDTH, WINDOW_HEIGHT);


    if(windowType==All)
    {
        renLeft.rect.x=0; renLeft.rect.y=0;
        renLeft.rect.w=WINDOW_WIDTH-400; renLeft.rect.h=WINDOW_HEIGHT/3;
        renLeft.start.x = 0; renLeft.start.y = renLeft.rect.y+renLeft.rect.h/2;

        renRight.rect.x=0; renRight.rect.y=WINDOW_HEIGHT/3;
        renRight.rect.w=WINDOW_WIDTH-400; renRight.rect.h=WINDOW_HEIGHT/3;
        renRight.start.x = 0; renRight.start.y = renRight.rect.y+renRight.rect.h/2;

        renXY.rect.x = renRight.rect.x+renRight.rect.w; renXY.rect.y=0;
        renXY.rect.w = 400; renXY.rect.h = 400;
        renXY.start.x = renXY.rect.x+200; renXY.start.y = 200;
    }
    else if (windowType==XY)
    {
        WINDOW.rect.x=0; WINDOW.rect.y=0;
        WINDOW.rect.w=WINDOW_WIDTH; WINDOW.rect.h=WINDOW_HEIGHT;
        WINDOW.start.x=WINDOW_WIDTH/2;WINDOW.start.y=WINDOW_HEIGHT/2;
    }
    else
    {
        WINDOW.rect.x=0; WINDOW.rect.y=0;
        WINDOW.rect.w=WINDOW_WIDTH; WINDOW.rect.h=WINDOW_HEIGHT;
        WINDOW.start.x=0;WINDOW.start.y=WINDOW_HEIGHT/2;
    }

    gDone=0;
}

gui::~gui() {
    //free(data);
    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
}

bool gui::update()
{
    SDL_Event e;
    if(SDL_PollEvent(&e))
    {
        // if(e.type == SDL_EVENT_WINDOW_RESIZED)
        //     {

        //     }
        if(e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            return false;
        if(e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE)
            return false;
    }

    char* pix;
    int pitch;

    SDL_LockTexture(gSDLTexture, NULL, (void**)&pix, &pitch);
    for(int i=0,sp=0, dp =0;i<WINDOW_HEIGHT;i++,dp+=WINDOW_WIDTH,sp+=pitch)
        memcpy(pix+sp, gFrameBuffer+dp, WINDOW_WIDTH*4);

    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, NULL, NULL);
    SDL_RenderPresent(gSDLRenderer);
    SDL_Delay(1);

    return true;
}

void gui::drawXY() {
    for (int i = 0; i < dataSize; i++) {
        SDL_RenderPresent(gSDLRenderer,scale(data[2*i]));
    }
}

void gui::changeDataPtr(int16_t*dataNew) {
    data=dataNew;
}

float gui::scale(int x, int start, int length, int max) {
    return float(start)+float(length)*float(x)/float(max);
}

