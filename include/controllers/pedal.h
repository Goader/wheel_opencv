#ifndef PEDAL_H
#define PEDAL_H

#include "../utils/keyboard.h"
#include "controller.h"
#include <iostream>

class Pedal : public Controller {
    std::string name;
    int code;  // virtual key code
    
    // int in a range of [0, 100], which says how hard the pedal is pressed
    // in fact, it can be either 0, 100 or int a range of [8, 92]
    int pressed;

    void run() const override;

public:
    explicit Pedal(std::string name);

    void setPressed(int pressed);
};

#endif  // PEDAL_H
