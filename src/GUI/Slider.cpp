#include "Slider.h"
#include "gui.h"
#include "SDL.h"
#include <iostream>

Slider::Slider(){
    clicked = false;
}

Slider::Slider(Gui* _parent, int _x_pos, int _y_pos, int _width, int _height, int size, const char* texturePath){
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 16;

    dstRect.x  = _x_pos;
    dstRect.y  = _y_pos+size;
    dstRect.w  = _width;
    dstRect.h  = _height;

    barOnSrc.x = barOffSrc.x = 0;
    barOnSrc.y = barOffSrc.y = 0;
    barOnSrc.w = barOffSrc.w = 24;
    barOnSrc.h = barOffSrc.h = 116;

    barOnDst.x = barOffDst.x = _x_pos+4;
    barOnDst.y = barOffDst.y = _y_pos;
    barOnDst.w = barOffDst.w = barOffSrc.w;
    barOnDst.h = barOffDst.h = barOffSrc.h;
    
    SDL_Surface* tmpSurface = IMG_Load("../../../assets/sliderBarON.png");
    barOn = SDL_CreateTextureFromSurface(parent->renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
    tmpSurface = IMG_Load("../../../assets/sliderBarOFF.png");
    barOff = SDL_CreateTextureFromSurface(parent->renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    setTexture(texturePath);
    clicked = false;
    value = 0.0;
    minY = _y_pos;
    maxY = _y_pos + size;
    range = size;

    _parent->addWidget(this);
}

Slider::~Slider(){

}

void Slider::bind(sliderFunc _function){
    func = _function;
}

void Slider::setValue(int _rvalX, int _rvalY){
    //double coefX = abs(1.0 + abs(_rvalX - (dstRect.x + dstRect.w))/100.f);
    //std::cout << "coef X :" << coefX << " | " << coefX*_rvalY << std::endl;
    if(_rvalY >= minY && _rvalY <= maxY){
        value = 1.0 - (double)(_rvalY-minY)/(double)(range);
        dstRect.y = _rvalY;
    }
}

void Slider::Update(){
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    if(    mouseX >= dstRect.x /*- dstRect.w */
        && mouseY >= dstRect.y - dstRect.h
        && mouseX < dstRect.x + dstRect.w 
        && mouseY < dstRect.y + dstRect.h ){
        //std::cout << "Mouse on Slider" << std::endl;
        if(!clicked) srcRect.x = 32;
        
        if(parent->event.type == SDL_MOUSEBUTTONDOWN && parent->event.button.button == SDL_BUTTON_LEFT){
            clicked = true;
        }
    }else{
        srcRect.x = 0;
    } 
    if(parent->event.type == SDL_MOUSEBUTTONUP && parent->event.button.button == SDL_BUTTON_LEFT){
        srcRect.x = 0;
        clicked = false;
    }

    if(clicked){
        srcRect.x = 2*32;
        setValue(mouseX, mouseY);
        func(value);

        barOnSrc.y  = 100*(1.f-value);
        barOnSrc.h  = 16 + 100*(value);
        barOffSrc.h = 16 + 100*(1.f-value);

        barOnDst.y  = maxY - (maxY-minY)*value;
        barOnDst.h  = 16 + 100*value;
        barOffDst.h = 16 + 100*(1.f-value);

    }   
}

bool Slider::isClicked(){
    return(clicked);
}

void Slider::Draw(){
    SDL_RenderCopy(parent->renderer,   barOn,  &barOnSrc,  &barOnDst);
    SDL_RenderCopy(parent->renderer,  barOff, &barOffSrc, &barOffDst);
    SDL_RenderCopy(parent->renderer, texture,   &srcRect,   &dstRect);
}