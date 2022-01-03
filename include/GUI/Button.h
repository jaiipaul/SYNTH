#ifndef _BUTTON_H_
#define _BUTTON_H_
#include <functional>
#include "Widget.h"


class Button : public Widget{
private:
    bool clicked;
    SDL_Event mouseEvent;
    
    buttonFunc func;

public :
    Button();
    Button(SDL_Renderer *_renderer, int _x_pos, int _y_pos, int _width, int _height, const char* texturePath);
    ~Button();

    void setFunction(buttonFunc);
    void Update();
    bool isClicked();
};

#endif //_BUTTON_H_