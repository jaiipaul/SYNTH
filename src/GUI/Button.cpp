#include "Button.h"
#include "gui.h"
#include "SDL.h"
#include <iostream>


Button::Button(){
    clicked = false;
}

Button::Button(SDL_Renderer *_renderer, int _x_pos, int _y_pos, int _width, int _height, const char* texturePath){
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 64;
    srcRect.h = 16;

    dstRect.x  = _x_pos;
    dstRect.y  = _y_pos;
    dstRect.w  = _width;
    dstRect.h  = _height;

    renderer = _renderer;
    setTexture(texturePath);
    std::cout << "texture loaded" << std::endl;
    clicked = false;
}

Button::~Button(){

}

void Button::setFunction(buttonFunc _function){
    func = _function;
}

void Button::Update(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    if(    mouseX >= dstRect.x 
        && mouseY >= dstRect.y 
        && mouseX < dstRect.x + dstRect.w 
        && mouseY < dstRect.y + dstRect.h ){
        
        if(!clicked) srcRect.x = 64;
        //std::cout << "Mouse on button" << std::endl;

        if(Gui::event.type == SDL_MOUSEBUTTONDOWN && Gui::event.button.button == SDL_BUTTON_LEFT){
            clicked = true;
            srcRect.x = 2*64;
            std::cout << "button clicked" << std::endl;
            func();
        }
    }else{
        srcRect.x = 0;
    }
    
    if(Gui::event.type == SDL_MOUSEBUTTONUP && Gui::event.button.button == SDL_BUTTON_LEFT){
        srcRect.x = 0;
        clicked = false;
    }
}

bool Button::isClicked(){
    return(clicked);
}
