#ifndef WHEELTRACKER_H
#define WHEELTRACKER_H

#include "controllertracker.h"
#include "../controllers/wheel.h"

class WheelTracker : public ControllerTracker {
    double centerX;
    double centerY;

    // IMPORTANT NOTE: wheel cannot have a bigger than 90-degree turn in each side
    double minAngle;
    double maxAngle;

    double calculateState(double posX, double posY) const override;

public:
    WheelTracker(Stream& stream, Wheel& pedal, std::string name) : ControllerTracker(stream, pedal, name) {}

    void init(std::string trackerTypeParam) override;  // TODO not the best argument name
};

#endif  // WHEELTRACKER_H
