#include "gui.h"
#include "Widgets.h"

SDL_Renderer* Gui::renderer = nullptr;
SDL_Event Gui::event;

Gui::Gui(){
}

Gui::~Gui(){
}

void Gui::init(const char* title, 
                int xpos, int ypos, 
                int width, int height, bool fullscreen){
//Init SDL window & renderer
    int FLAGS = 0;
    if(fullscreen){
        FLAGS = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "subsystems initialised!..." << std::endl;
        
        //windowWidth  = width;
        //windowHeight = height;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, FLAGS);
        if(window){
            std::cout << "window created!..." << std::endl;
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 150, 100, 150, 255);
            std::cout << "renderer created!..." << std::endl;
        }
        Running = true;
    }   
}

void Gui::handleEvents(){
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:
            Running = false;
            break;

        default:
            break;
    }
}

void Gui::update(){
    for(auto w : Widgets){
        w->Update();
    }
}

void Gui::render(){
    SDL_RenderClear(renderer);
    int i = 0;
    //Where to add stuff to render
    for(auto w : Widgets){
        w->Draw();
    }
    SDL_RenderPresent(renderer);
}

void Gui::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Gui cleaned!..." << std::endl;
}

void Gui::addWidget(Widget* widget){
    Widgets.push_back(widget);
}

