#include "../../include/controllers/pedal.h"

Pedal::Pedal(std::string name) : name{name}, pressed{0} {
    if (name == "gas") code = 0x57;  // W key
    else if (name == "brake") code = 0x53;  // S key
    else throw std::invalid_argument("The name must be either 'gas' or 'break'!");
}

void Pedal::setState(double pressedValue) {
    if (pressedValue < 8) pressed = 0;
    else if (pressedValue > 92) pressed = 100;
    else pressed = (int)pressedValue;
}

void Pedal::run() const {
    bool released = true;
    while (this->active) {
        if (pressed == 100 && released) {
            press(this->code);
            released = false;
            Sleep(50);
        }
        else if (pressed == 0 && !released) {
            release(this->code);
            released = true;
            Sleep(50);
        }
        else if (pressed != 0 && pressed != 100) {
            press(this->code);
            released = false;
            
            Sleep(100);

            release(this->code);
            released = true;

            Sleep(120 - pressed);  // '100 - pressed' will always be from [8, 92] interval
        }
        else Sleep(20);
    }
}
