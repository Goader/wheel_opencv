#include "../../include/controllers/wheel.h"

void Wheel::setTurned(int angle) {
    if (angle < -180) angle = -180;
    else if (angle > 180) angle = 180;

    this->angle = angle;
}

void Wheel::run() const {
    // TODO invent a way on how to send turns
    // can be pressing the mouse, and changing its x value (horizontal movement)
    // or it can be as with the gas and brake (pressing A and D in some time periods)
}
