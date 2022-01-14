#include "Button.h"
#include "gui.h"
#include "SDL.h"
#include <iostream>



Button::Button(){
    clicked = false;
}

Button::Button(Gui* _parent, int _x_pos, int _y_pos, int _width, int _height, const char* texturePath){
    parent = _parent;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    dstRect.x  = _x_pos;
    dstRect.y  = _y_pos;
    dstRect.w  = _width;
    dstRect.h  = _height;

    setTexture(texturePath);
    clicked = false;

    _parent->addWidget(this);
}

Button::~Button(){

}

void Button::bind(buttonFunc _function){
    func = _function;
}

void Button::Update(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    if(    mouseX >= dstRect.x 
        && mouseY >= dstRect.y 
        && mouseX < dstRect.x + dstRect.w 
        && mouseY < dstRect.y + dstRect.h ){
        
        if(!clicked) srcRect.x = 32;
        //std::cout << "Mouse on button" << std::endl;

        if(parent->event.type == SDL_MOUSEBUTTONDOWN && parent->event.button.button == SDL_BUTTON_LEFT){
            clicked = true;
            srcRect.x = 2*32;
            std::cout << "button clicked" << std::endl;
            func();
        }
    }else{
        srcRect.x = 0;
    }
    
    if(parent->event.type == SDL_MOUSEBUTTONUP && parent->event.button.button == SDL_BUTTON_LEFT){
        srcRect.x = 0;
        clicked = false;
    }
}

bool Button::isClicked(){
    return(clicked);
}

void Button::Draw(){
    SDL_RenderCopy(parent->renderer, texture, &srcRect, &dstRect);
}