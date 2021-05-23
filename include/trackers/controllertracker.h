#ifndef CONTROLLERTRACKER_H
#define CONTROLLERTRACKER_H

#define _USE_MATH_DEFINES  // lets cmath define PI value

#include "../utils/stream.h"
#include "../controllers/controller.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <thread>
#include <cmath>
#include <iostream>

class ControllerTracker {
protected:
    std::shared_ptr<Stream> stream;
    std::shared_ptr<Controller> controller;

    std::string trackerType;
    cv::Ptr<cv::legacy::Tracker> tracker;

    std::string name;

    static int bufferX;
    static int bufferY;

    static void onClick(int event, int x, int y, int flags, void* param);

    virtual double calculateState(double posX, double posY) const = 0;

public:
    ControllerTracker(Stream& stream, Controller& controller, std::string name) : 
        stream{&stream}, controller{&controller}, name{name} {}

    virtual void init(std::string trackerType) = 0;

    void track() const;

    virtual ~ControllerTracker() {}
};

#endif  // CONTROLLERTRACKER_H
