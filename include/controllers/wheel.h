#ifndef WHEEL_H
#define WHEEL_H

#include "controller.h"

class Wheel : public Controller {
    // angle attribute can have out of [-180, 180] range (starting from the left turn to the right)
    int angle;

    void run() const override;
public:
    void setTurned(int angle);
};

#endif  // WHEEL_H
