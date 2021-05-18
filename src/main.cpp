#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <iostream>
#include <thread>

#include "../include/controllers/pedal.h"
#include "../include/trackers/pedaltracker.h"

int main() {
    cv::VideoCapture cap = cv::VideoCapture(0);
    if (!cap.isOpened()) {
        throw std::system_error(ENODEV, std::generic_category(), "Cannot access the device");
    }

    // setting up pedals
    Pedal gasPedal("gas");
    Pedal brakePedal("brake");

    std::thread gPedalThread(&Pedal::start, &gasPedal);
    std::thread bPedalThread(&Pedal::start, &brakePedal);

    // setting up pedal trackers
    PedalTracker gasPedalTracker(cap, gasPedal, "Gas");
    PedalTracker brakePedalTracker(cap, brakePedal, "Brake");

    gasPedalTracker.init("MedianFlow");
    brakePedalTracker.init("MedianFlow");

    std::thread gTrackerThread(&PedalTracker::track, &gasPedalTracker);
    // std::thread bTrackerThread(&PedalTracker::track, &brakePedalTracker);
    
    // TODO create a stream class, which will produce frames from the camera,
    // and then using Mutexes supply thread safe API for getting frames
    // in fact, solving producer-consumer problem, having one producer and multiple consumers 

    // waiting for trackers to complete their jobs
    gTrackerThread.join();
    // bTrackerThread.join();

    // trackers are down -> shutting down pedals
    gasPedal.shut_down();
    brakePedal.shut_down();

    cap.release();
    cv::destroyAllWindows();

    gPedalThread.join();
    bPedalThread.join();

    return 0;
}
