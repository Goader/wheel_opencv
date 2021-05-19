#ifndef PEDALTRACKER_H
#define PEDALTRACKER_H

#include "controllertracker.h"
#include "../controllers/pedal.h"

class PedalTracker : public ControllerTracker {
    double minY;
    double maxY;

    double calculateState(double posX, double posY) const override;

public:
    PedalTracker(Stream& stream, Pedal& pedal, std::string name) : ControllerTracker(stream, pedal, name) {}

    void init(std::string trackerType) override;
};

#endif  // PEDALTRACKER_H
