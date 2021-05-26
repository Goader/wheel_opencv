#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>

#include "../include/controllers/pedal.h"
#include "../include/controllers/wheel.h"
#include "../include/trackers/wheeltracker.h"
#include "../include/trackers/pedaltracker.h"
#include "../include/utils/stream.h"

int main() {
    // setting up streams
    // NOTE: camera indices may change depending on your current setup
    // in my case: 1 - OBS Virtual Camera, 0 - DroidCam
    Stream pedalStream(2);
    std::cout << "Pedal stream camera found!" << std::endl;
    Stream wheelStream(4);
    std::cout << "Wheel stream camera found!" << std::endl;
    
    std::thread pedalStreamThread(&Stream::start, &pedalStream);
    std::thread wheelStreamThread(&Stream::start, &wheelStream);

    //setting up the wheel and the pedals
    Wheel wheel;
    Pedal gasPedal("gas");
    Pedal brakePedal("brake");

    std::thread wheelThread(&Wheel::start, &wheel);
    std::thread gPedalThread(&Pedal::start, &gasPedal);
    std::thread bPedalThread(&Pedal::start, &brakePedal);

    // setting up the trackers
    WheelTracker wheelTracker(wheelStream, wheel, "Wheel");
    PedalTracker gasPedalTracker(pedalStream, gasPedal, "Gas");
    PedalTracker brakePedalTracker(pedalStream, brakePedal, "Brake");

    wheelTracker.init("CSRT");
    gasPedalTracker.init("CSRT");
    brakePedalTracker.init("CSRT");

    std::thread wTrackerThread(&WheelTracker::track, &wheelTracker);
    std::thread gTrackerThread(&PedalTracker::track, &gasPedalTracker);
    std::thread bTrackerThread(&PedalTracker::track, &brakePedalTracker);

    // waiting for trackers to complete their jobs
    wTrackerThread.join();
    gTrackerThread.join();
    bTrackerThread.join();

    cv::destroyAllWindows();

    // shutting down streams
    wheelStream.shutDown();
    pedalStream.shutDown();

    // trackers are down -> shutting down pedals and the wheel
    wheel.shutDown();
    gasPedal.shutDown();
    brakePedal.shutDown();

    // joining all the threads
    wheelThread.join();
    gPedalThread.join();
    bPedalThread.join();

    wheelStreamThread.join();
    pedalStreamThread.join();

    return 0;
}
