#ifndef PEDALTRACKER_H
#define PEDALTRACKER_H

#include "controllertracker.h"
#include "../controllers/pedal.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <thread>
#include <iostream>

class PedalTracker : public ControllerTracker {
    std::shared_ptr<cv::VideoCapture> cap;
    std::shared_ptr<Pedal> pedal;

    std::string trackerType;
    cv::Ptr<cv::legacy::Tracker> tracker;

    std::string name;

    static int bufferY;

    int minY;
    int maxY;

    static void onClick(int event, int x, int y, int flags, void* param);

public:
    PedalTracker(cv::VideoCapture& cap, Pedal& pedal, std::string name) : 
        cap{&cap}, pedal{&pedal}, name{name} {}

    void init(std::string trackerType);

    void track() const;
};

#endif  // PEDALTRACKER_H
