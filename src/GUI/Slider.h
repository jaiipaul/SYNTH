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
    Slider(Gui* _parent, int _x_pos, int _y_pos, int _width, int _height, int size, const char* texturePath);
    ~Slider();

    void bind(sliderFunc _function);
    void setValue(int _rval);
    int  getValue();
    void Update();
    bool isClicked();
};

#endif //_BUTTON_H_