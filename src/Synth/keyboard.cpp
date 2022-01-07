#include "keyboard.h"
#include "SDL.h"
#include "gui.h"
#include <math.h>
Keyboard::Keyboard(){
    octave = 3;
    Gate    = false;
    Trigger = false;
}

Keyboard::~Keyboard(){

}

void Keyboard::Update(){
    if(Gui::event.type == SDL_KEYDOWN){
        switch (Gui::event.key.keysym.sym){
            case SDLK_q:
                note = pow(2.0f, 0.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_z:
                note = pow(2.0f, 1.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_s:
                note = pow(2.0f, 2.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_e:
                note = pow(2.0f, 3.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_d:
                note = pow(2.0f, 4.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_f:
                note = pow(2.0f, 5.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_t:
                note = pow(2.0f, 6.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_g:
                note = pow(2.0f, 7.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_y:
                note = pow(2.0f, 8.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_h:
                note = pow(2.0f, 9.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_u:
                note = pow(2.0f, 10.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_j:
                note = pow(2.0f, 11.0f/12.0f) * pow(2, octave);
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            case SDLK_w:
                if(octave > 0) octave--;
                std::cout << "octave : " << octave << std::endl;
                break;
            case SDLK_x:
                if(octave < 7) octave++;
                std::cout << "octave : " << octave << std::endl;
                break;
            default:
                break;
        }
    }else if(Gui::event.type == SDL_KEYUP){
        switch (Gui::event.key.keysym.sym){
            case SDLK_q:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_z:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_s:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_e:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_d:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_f:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_t:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_g:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_y:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_h:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_u:
                Gate = false;
                Trigger = false;
                break;
            case SDLK_j:
                Gate = false;
                Trigger = false;
                break;
            default:
                break;
        }
    }
    //std::cout << Trigger << std::endl;
}

float Keyboard::getNote(){
    return(note);
}

bool Keyboard::getGate(){
    return Gate;
}

bool Keyboard::getTrigger(){
    return Trigger;
}