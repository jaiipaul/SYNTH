#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

class Keyboard{
private:
    int baseOctave = 3;
    int octave;
    float note;

    bool Gate;
    bool Trigger;

public:
    Keyboard();
    ~Keyboard();

    void  Update();
    float getNote();
    bool  getGate();
    bool  getTrigger();
};
#endif // _KEYBOARD_H_