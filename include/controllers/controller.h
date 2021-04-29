#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
protected:
    bool active;
    virtual void run() const = 0;
public:
    void start() {
        active = true;
        run();
    }

    void shut_down() {
        active = false;
    }

    virtual ~Controller() {
        active = false;
    }
};

#endif  // CONTROLLER_H
