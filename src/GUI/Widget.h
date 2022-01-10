#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "SDL.h"

#include "typeDef.h"

class Gui;

class Widget{
public:
    Gui* parent;

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