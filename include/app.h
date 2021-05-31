#ifndef APP_H
#define APP_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>

#include "controllers/pedal.h"
#include "controllers/wheel.h"
#include "trackers/wheeltracker.h"
#include "trackers/pedaltracker.h"
#include "utils/stream.h"
#include "utils/config.h"

class App {
    Config config;
public:
    App() : config{} {
        config.load();
    }

    void run() const;

};

#endif  // APP_H
