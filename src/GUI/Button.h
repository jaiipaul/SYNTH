#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Widget.h"


//template<class Module>
class Button : public Widget{
private:
    bool clicked;
    SDL_Event mouseEvent;
    
    buttonFunc func;

public :
    Button();
    Button(Gui* _parent, int _x_pos, int _y_pos, int _width, int _height, const char* texturePath);
    ~Button();

    void bind(buttonFunc _func);
    void operator > (buttonFunc _func){
        bind(_func);
    }
    
    void Update();
    bool isClicked();
};

#endif //_BUTTON_H_