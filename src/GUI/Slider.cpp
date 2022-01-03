#include "Slider.h"
#include "gui.h"
#include "SDL.h"
#include <iostream>

Slider::Slider(){
    clicked = false;
}

Slider::Slider(SDL_Renderer *_renderer, int _x_pos, int _y_pos, int _width, int _height, int size, const char* texturePath){
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 64;
    srcRect.h = 16;

    dstRect.x  = _x_pos;
    dstRect.y  = _y_pos+_height;
    dstRect.w  = _width;
    dstRect.h  = _height;

    renderer = _renderer;
    setTexture(texturePath);
    std::cout << "texture loaded" << std::endl;
    clicked = false;
    value = 0.0;
    minY = _y_pos;
    maxY = _y_pos + size;
    range = size;
}

Slider::~Slider(){

}

void Slider::setFunction(sliderFunc _function){
    func = _function;
}

void Slider::setValue(int _rval){
    if( _rval >= minY && _rval <= maxY){
        value = 1.0 - (double)(_rval-minY)/(double)(range);
        dstRect.y = _rval;
    }
}

void Slider::Update(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    if(    mouseX >= dstRect.x - dstRect.w 
        && mouseY >= dstRect.y - dstRect.h
        && mouseX < dstRect.x + dstRect.w 
        && mouseY < dstRect.y + dstRect.h ){
        //std::cout << "Mouse on Slider" << std::endl;
        if(!clicked) srcRect.x = 64;
        
        if(Gui::event.type == SDL_MOUSEBUTTONDOWN && Gui::event.button.button == SDL_BUTTON_LEFT){
            clicked = true;
        }
    }else{
        srcRect.x = 0;
    }
    if(Gui::event.type == SDL_MOUSEBUTTONUP && Gui::event.button.button == SDL_BUTTON_LEFT){
        srcRect.x = 0;
        clicked = false;
    }

    if(clicked){
        srcRect.x = 2*64;
        setValue(mouseY);
        func(value);
    }   
}

bool Slider::isClicked(){
    return(clicked);
}
