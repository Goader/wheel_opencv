#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../utils/keyboard.h"

class Controller {
protected:
    bool active;
    virtual void run() const = 0;
public:
    void start() {
        active = true;
        run();
    }

    virtual void setState(double state) = 0;

    void shut_down() {
        active = false;
    }

    virtual ~Controller() {
        active = false;
    }
};

#endif  // CONTROLLER_H
