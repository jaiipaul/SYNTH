#ifndef _GUI_H_
#define _GUI_H_

#include "SDL_image.h"
#include "SDL.h"

#include <iostream>
#include <vector>

class Widget;

class Gui{
protected:
    bool Running = true;
    SDL_Window *window;

    std::vector<Widget*> Widgets;  
public:
    Gui();
    ~Gui();

    void init(const char* title, 
              int xpos, int ypos, 
              int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool isRunning() { return Running; }

    void addWidget(Widget* widget);

    static SDL_Renderer* renderer;
    static SDL_Event event;
    //static int windowWidth;
    //static int windowHeight;
    
};

#endif // _GUI_H_