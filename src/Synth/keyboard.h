#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <libremidi/libremidi.hpp>
#include <SDL.h>

class Keyboard{
private:
    int baseOctave = 3;
    int octave;
    float note;

    SDL_Keycode Last_KeyNote;
    unsigned char Last_MidiNote;

    bool Gate;
    bool Trigger;

    libremidi::midi_in* midiIn;
    int midiPort; 
    
public:
    Keyboard();
    ~Keyboard();

    void init(bool midi);

    void  Update();
    void  MidiUpdate();
    void  KeyUpdate();
    float getNote();
    bool  getGate();
    bool  getTrigger();

    void setMidiPort(int port);
};
#endif // _KEYBOARD_H_