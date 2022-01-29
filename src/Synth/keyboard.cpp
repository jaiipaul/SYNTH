#include "keyboard.h"
#include "SDL.h"
#include "gui.h"
#include <math.h>
#include <iostream>
#include <stdio.h>

Keyboard::Keyboard(){
    
}

Keyboard::~Keyboard(){

}

void Keyboard::init(bool midi){
    midiIn = new libremidi::midi_in();
    if(midi && (midiIn->get_port_count() > 0)){
        std::cout << "Midi : " << midiIn->get_port_count() << " ports available" << std::endl;
        midiPort = 0;
        midiIn->open_port(midiPort);
    }else{
        std::cout << "No midi ports available" << std::endl;
    }

    octave = 3;
    Gate    = false;
    Trigger = false;
}

void Keyboard::Update(){
    if(midiIn->is_port_open()){
        MidiUpdate();
    }else{
        KeyUpdate();
    }
}

void Keyboard::MidiUpdate(){
    libremidi::message messageIn;
    if(midiIn->get_message(messageIn)){
        printf("%x\n", messageIn.bytes[0]);
        float mod = fmod((float)messageIn.bytes[1], 12.f);
        float oct = messageIn.bytes[1]/ 12;
        printf("mod : %f | oct : %f\n", mod, oct);
        switch (messageIn.bytes[0]){
            case 0x90:
                Last_MidiNote = messageIn.bytes[1];
                note = pow(2.0f, (fmod((float)messageIn.bytes[1], 12.f)/12.0f)) * pow(2, (int)messageIn.bytes[1]/12);
                std::cout << "Trig ON" << std::endl;
                Trigger = !Gate ? true : false;
                Gate = true;
                break;
            
            case 0x80:
                if(messageIn.bytes[1] == Last_MidiNote){
                    Gate = false;
                    Trigger = false;
                }
                break;
            
        }
        
        printf("%f\n", note); 
    
        //for(auto b : messageIn.bytes){
        //    printf("%x | ", b);
        //}
        //printf("\n");        
    }else if(Trigger){
        Trigger = false;
        std::cout << "Trig OFF" << std::endl;
    }
}

void Keyboard::KeyUpdate(){
    if(Gui::event.type == SDL_KEYDOWN){
        Last_KeyNote = Gui::event.key.keysym.sym;
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
    }else if(Gui::event.type == SDL_KEYUP && Gui::event.key.keysym.sym == Last_KeyNote){
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

void Keyboard::setMidiPort(int port){
    if(port <= midiIn->get_port_count()-1){
        midiPort = port;
        midiIn->open_port(midiPort);
    }else{
        std::cout << "Port not available" << std::endl;
    } 
}