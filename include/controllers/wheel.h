#ifndef WHEEL_H
#define WHEEL_H

#include "controller.h"
#include <cmath>

class Wheel : public Controller {
    // angle attribute can have out of [-180, 180] range (starting from the left turn to the right)
    std::atomic_int angle;

    const int leftCode = 0x41;  // A
    const int rightCode = 0x44;  // D

    void run() const override;
public:
    Wheel() : angle{0} {}

    void setState(double angleValue) override;
};

#endif  // WHEEL_H
