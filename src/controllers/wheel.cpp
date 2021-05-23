#include "../../include/controllers/wheel.h"

void Wheel::setState(double angleValue) {
    if (angleValue < -170) angle = -180;
    else if (angleValue > 170) angle = 180;
    else if (abs(angleValue) < 10) angle = 0;
    else angle = (int)angleValue;
}

void Wheel::run() const {
    // a way on how to send turns
    // can be pressing the mouse, and changing its x value (horizontal movement)
    // or it can be as with the gas and brake (pressing A and D in some time periods)
    bool leftReleased = true;
    bool rightReleased = true;
    while (this->active) {
        if (angle == 180 && rightReleased) {
            press(rightCode);
            rightReleased = false;
            Sleep(50);
        }
        else if (angle == -180 && leftReleased) {
            press(leftCode);
            leftReleased = false;
            Sleep(50);
        }
        else if (angle == 0 && (!leftReleased || !rightReleased)) {
            release(leftCode);
            release(rightCode);
            leftReleased = rightReleased = true;
            Sleep(50);
        }
        else if (angle < 0 && angle != -180) {
            release(rightCode);
            rightReleased = true;

            press(leftCode);
            leftReleased = false;
            
            Sleep(13);

            release(leftCode);
            leftReleased = true;

            Sleep(50 + (25 * angle) / 180);
        }
        else if (angle > 0 && angle != 180) {
            release(leftCode);
            leftReleased = true;

            press(rightCode);
            rightReleased = false;
            
            Sleep(13);

            release(rightCode);
            rightReleased = true;

            Sleep(50 - (25 * angle - 10) / 180);
        }
        else Sleep(20);
    }
}
