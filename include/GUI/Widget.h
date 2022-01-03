#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "SDL.h"

typedef void (*buttonFunc)();
typedef void (*sliderFunc)(double);

class Widget{
public:
    SDL_Renderer *renderer;
    SDL_Texture  *texture;
    SDL_Rect srcRect, dstRect;
    
public:
    Widget();
    ~Widget();

    
    void setTexture(const char* TexturePath);
    virtual void Update()=0;
    void Draw();
};

#endif //_WIDGET_H_