#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "Widget.h"


class Slider : public Widget{
private:
    bool    clicked;
    double  value;
    SDL_Event mouseEvent;
    
    int range = 1000;
    int minY;
    int maxY;

    sliderFunc func;

public :
    Slider();
    Slider(SDL_Renderer *_renderer, int _x_pos, int _y_pos, int _width, int _height, int size, const char* texturePath);
    ~Slider();

    void setFunction(sliderFunc func);
    void setValue(int _rval);
    int  getValue();
    void Update();
    bool isClicked();
};

#endif //_BUTTON_H_