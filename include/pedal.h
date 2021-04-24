#ifndef PEDAL_H
#define PEDAL_H

#include "../include/keyboard.h"
#include <iostream>
#include <thread>

class Pedal {
    std::string name;
    int code;  // virtual key code

    bool active;
    
    // int in a range of [0, 100], which says how hard the pedal is pressed
    // in fact, it can be either 0, 100 or int a range of [8, 92]
    int pressed;

    void run();

public:
    explicit Pedal(std::string name);

    void start();

    void shut_down() {
        active = false;
    }

    void setPressed(int pressed);

    ~Pedal() {
        active = false;
    }
};

#endif  // PEDAL_H
