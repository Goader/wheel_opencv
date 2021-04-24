#include "../include/pedal.h"

Pedal::Pedal(std::string name) : name{name}, pressed{0} {
    if (name == "gas") code = 0x57;  // W key
    else if (name == "brake") code = 0x53;  // S key
    else throw std::invalid_argument("The name must be either 'gas' or 'break'!");
}

void Pedal::start() {
    active = true;
    // std::thread t(&Pedal::run, this);
    run();
}

void Pedal::setPressed(int pressed) {
    if (pressed < 8) pressed = 0;
    else if (pressed > 92) pressed = 100;

    this->pressed = pressed;
}

void Pedal::run() {
    bool released = true;
    while (this->active) {
        // std::cout << "still running" << pressed << std::endl;
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
    }
}