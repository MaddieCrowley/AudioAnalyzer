#include "sdl.h"

gui::gui(winType windowT, int width, int height, const char* title, int16_t*data,int dataSize,int stepSize)
    : WINDOW_WIDTH(width),WINDOW_HEIGHT(height),windowType(windowT),
      title(title),data(data),dataSize(dataSize),m_stepSize(stepSize){

    gFrameBuffer = new int[WINDOW_WIDTH * WINDOW_HEIGHT];
    //int gFrameBuffer[WINDOW_WIDTH*WINDOW_HEIGHT] = {};
    gSDLWindow = SDL_CreateWindow(title, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, nullptr);
    gSDLTexture = SDL_CreateTexture(gSDLRenderer,
                                    SDL_PIXELFORMAT_ABGR8888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    WINDOW_WIDTH, WINDOW_HEIGHT);


    if(windowType==All) //TODO Clean this up, must be a way
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
    else if (windowType==LR) {
        renLeft.rect.x=0; renLeft.rect.y=0;
        renLeft.rect.w=WINDOW_WIDTH; renLeft.rect.h=WINDOW_HEIGHT/2;
        renLeft.start.x = 0; renLeft.start.y = renLeft.rect.y+renLeft.rect.h/2;

        renRight.rect.x=0; renRight.rect.y=renLeft.rect.h;
        renRight.rect.w=WINDOW_WIDTH; renRight.rect.h=WINDOW_HEIGHT/2;
        renRight.start.x = 0; renRight.start.y = renRight.rect.y+renRight.rect.h/2;
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
    delete[] gFrameBuffer;
    //free(data);
    SDL_DestroyTexture(gSDLTexture);
    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
}

bool gui::update() const {
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

    SDL_LockTexture(gSDLTexture, nullptr, (void**)&pix, &pitch);
    for(int i=0,sp=0, dp =0;i<WINDOW_HEIGHT;i++,dp+=WINDOW_WIDTH,sp+=pitch)
        memcpy(pix+sp, gFrameBuffer+dp, WINDOW_WIDTH*4);

    SDL_UnlockTexture(gSDLTexture);
    SDL_RenderTexture(gSDLRenderer, gSDLTexture, nullptr, nullptr);
    SDL_RenderPresent(gSDLRenderer);
    //SDL_Delay(5);
    //saves SOOOOO much CPU usage (going from none to 5ms literally halves it) with no perceptible difference

    return true;
}

void gui::loop() {
#ifdef FRAME_RATE
    uint32_t start = SDL_GetTicks();
#endif
    if (!update())
        gDone=1;
    else
        render();
#ifdef FRAME_RATE
    uint32_t end = SDL_GetTicks();
    float delta = (float)(end-start)/1000.0f;
    std::cout << float(1/delta) << "\n";
#endif
}


void gui::render() {
    //CLEAR BACKGROUND AND SET COLOR
    SDL_SetRenderDrawColor(gSDLRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gSDLRenderer);
    SDL_SetRenderDrawColor(gSDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    if(windowType==XY)
        drawXY(WINDOW.rect,WINDOW.start);
    else if(windowType==L)
        drawLineL(WINDOW.rect,WINDOW.start);
    else if(windowType==R)
        drawLineR(WINDOW.rect,WINDOW.start);
    else if(windowType==LR) {
        drawLineL(renLeft.rect,renLeft.start);
        drawLineR(renRight.rect,renRight.start);
    }
    else if(windowType==All) {
        drawXY(renXY.rect,renXY.start);
        drawLineL(renLeft.rect,renLeft.start);
        drawLineR(renRight.rect,renRight.start);
    }

}

void gui::drawXY(SDL_Rect&region,SDL_Point&start) { //Draws from member local pointer data (no copies)
    for (int i = 0; i < dataSize/2; i+=m_stepSize) {
        SDL_RenderPoint(gSDLRenderer,
            scale(data[2*i],start.x,region.w,2*INT16_MAX), //2x b/c center aligned
            scale(data[2*i+1],start.y,region.h,2*INT16_MAX)
            );
    }
}

void gui::drawLineL(SDL_Rect&region,SDL_Point&start) {
    for (int i = m_stepSize; i < dataSize/2; i+=m_stepSize) {
        SDL_RenderLine(gSDLRenderer,
            scale(i-m_stepSize,start.x,region.w,dataSize/2),
            scale(data[2*(i-m_stepSize)],start.y,region.h,2*INT16_MAX),
            scale(i,start.x,region.w,dataSize/2),
            scale(data[2*i],start.y,region.h,2*INT16_MAX)
            );
    }
}
void gui::drawLineR(SDL_Rect&region,SDL_Point&start) {
    for (int i = m_stepSize; i < dataSize/2; i+=m_stepSize) {
        SDL_RenderLine(gSDLRenderer,
            scale(i-m_stepSize,start.x,region.w,dataSize/2),
            scale(data[2*(i-m_stepSize)+1],start.y,region.h,2*INT16_MAX),
            scale(i,start.x,region.w,dataSize/2),
            scale(data[2*i+1],start.y,region.h,2*INT16_MAX)
            );
    }
}

void gui::changeDataPtr(int16_t*dataNew) {

    //TODO FINISH This mess

    if (dataSize>AUDIO_SIZE*AUDIO_CHANNELS) {
        const int end = dataSize-1;
        memcpy(data,data+AUDIO_SIZE*AUDIO_CHANNELS,(dataSize-AUDIO_SIZE*AUDIO_CHANNELS)*sizeof(int16_t));
        memcpy(data+end-AUDIO_CHANNELS*AUDIO_SIZE,dataNew,AUDIO_SIZE*AUDIO_CHANNELS*sizeof(int16_t));
    }
    else {
        data=dataNew;
    }
}
//TODO instead of calculating length/max every time, calculate once and multiply (faster?)
// INLINE FASTER?
float gui::scale(int x, int start, int length, int max) {
    return float(start)+float(length)*float(x)/float(max);
}

