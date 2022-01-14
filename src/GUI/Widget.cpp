#include "Widget.h"
#include "gui.h"
#include "SDL_image.h"

Widget::Widget(){

}

Widget::~Widget(){

}

void Widget::setTexture(const char* TexturePath){
    SDL_Surface* tmpSurface = IMG_Load(TexturePath);
    texture = SDL_CreateTextureFromSurface(parent->renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);
}

void Widget::Update(){

}
