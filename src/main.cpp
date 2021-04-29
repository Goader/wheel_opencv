#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
#include <iostream>
#include <thread>

#include "../include/controllers/pedal.h"

int main() {

    std::string tracker_type = "MEDIANFLOW";
    // auto trackerGas = cv::legacy::TrackerMedianFlow::create();
    // auto trackerBrake = cv::legacy::TrackerMedianFlow::create();

    auto trackerGas = cv::TrackerCSRT::create();
    auto trackerBrake = cv::TrackerCSRT::create();
    
    // auto trackerGas = cv::TrackerGOTURN::create();
    // auto trackerBrake = cv::TrackerGOTURN::create();

    // auto tracker = cv::legacy::TrackerMOSSE::create();

    cv::VideoCapture cap = cv::VideoCapture(0);

    if (!cap.isOpened()) {
        throw std::system_error(ENODEV, std::generic_category(), "Cannot access the device");
    }

    cv::Mat frame;
    bool ok;

    // reading in a few frames
    for (int i = 0; i < 10; i++) {
        ok = cap.read(frame);
        if (!ok)
            throw std::system_error(ENODEV, std::generic_category(), "Frame cannot be read from device");
    }

    // cv::Rect bbox = cv::selectROI(frame, true);
    cv::Rect bboxGas = cv::selectROI(frame, true);
    // cv::Rect2d bboxGas = cv::selectROI(frame, true);
    cv::destroyAllWindows();

    cv::Rect bboxBrake = cv::selectROI(frame, true);
    // cv::Rect2d bboxBrake = cv::selectROI(frame, true);
    cv::destroyAllWindows();

    // auto tracker = cv::legacy::MultiTracker::create();
    // tracker->add(trackerGas, frame, bboxGas);
    // tracker->add(trackerBrake, frame, bboxBrake);

    trackerGas->init(frame, bboxGas);
    trackerBrake->init(frame, bboxBrake);

    Sleep(4000);
    std::cout << "Get ready!" << std::endl;
    Sleep(1000);

    Pedal gasPedal("gas");
    std::thread t1(&Pedal::start, &gasPedal);

    Pedal brakePedal("brake");
    std::thread t2(&Pedal::start, &brakePedal);

    while (true) {
        ok = cap.read(frame);
        if (!ok) break;
        
        int64 timer = cv::getTickCount();

        bool okGas = trackerGas->update(frame, bboxGas);
        bool okBrake = trackerBrake->update(frame, bboxBrake);
        // ok = tracker->update(frame);

        std::cout << bboxGas.y + bboxGas.height / 2 << std::endl;

        // bboxGas = tracker->getObjects()[0];
        // bboxBrake = tracker->getObjects()[1];
        // strongly depends on the way you set up camera
        // pedal.setPressed((bbox.y + bbox.height / 2 - 220) / 2);
        gasPedal.setPressed((bboxGas.y + bboxGas.height / 2 - 160) / 2);
        brakePedal.setPressed((bboxBrake.y + bboxBrake.height / 2 - 160) / 2);

        std::cout << bboxGas.y + bboxGas.height / 2 << std::endl;

        auto fps = cv::getTickFrequency() / (cv::getTickCount() - timer);

        // showing gas bbox
        if (okGas)
            cv::rectangle(frame, bboxGas, cv::Scalar(255, 0, 0), 2, 1);
        else 
            cv::putText(frame, "Tracking failure detected", cv::Point(100, 80), 
                        cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

        // showing brake bbox
        if (okGas) {
            cv::rectangle(frame, bboxBrake, cv::Scalar(0, 255, 0), 2, 1);
        }
        else 
            cv::putText(frame, "Tracking failure detected", cv::Point(100, 80), 
                        cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

        // legend
        cv::putText(frame, tracker_type + " Tracker", cv::Point(100, 20), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::putText(frame, "FPS : " + std::to_string(fps), cv::Point(100, 50), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::imshow("Tracking", frame);

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }

    // pedal.shut_down();
    gasPedal.shut_down();
    brakePedal.shut_down();

    cap.release();
    cv::destroyAllWindows();

    t1.join();
    t2.join();
    Sleep(2000);
}
