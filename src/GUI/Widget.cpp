#include "Widget.h"
#include "SDL_image.h"

Widget::Widget(){

}

Widget::~Widget(){

}

void Widget::setTexture(const char* TexturePath){
    SDL_Surface* tmpSurface = IMG_Load(TexturePath);
    texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void Widget::Update(){

}

void Widget::Draw(){
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}