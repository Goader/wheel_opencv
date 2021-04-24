#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/tracking.hpp>
#include <iostream>
#include <thread>

#include "../include/pedal.h"

int main() {

    std::string tracker_type = "CSRT";
    auto tracker = cv::TrackerCSRT::create();

    // std::string filepath = "C:/FILES_IN_USE/prj/wheel/videos/chaplin.mp4";
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

    cv::Rect bbox = cv::selectROI(frame, true);

    tracker->init(frame, bbox);

    cv::destroyAllWindows();

    Sleep(4000);
    std::cout << "Get ready!" << std::endl;
    Sleep(1000);

    Pedal pedal("gas");
    std::thread t(&Pedal::start, &pedal);

    while (true) {
        ok = cap.read(frame);
        if (!ok) break;

        int64 timer = cv::getTickCount();

        ok = tracker->update(frame, bbox);

        // strongly depends on the way you set up camera
        pedal.setPressed((bbox.y + bbox.height / 2 - 220) / 2);

        auto fps = cv::getTickFrequency() / (cv::getTickCount() - timer);

        if (ok)
            cv::rectangle(frame, bbox, cv::Scalar(255, 0, 0), 2, 1);
        else 
            cv::putText(frame, "Tracking failure detected", cv::Point(100, 80), 
                        cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

        cv::putText(frame, tracker_type + " Tracker", cv::Point(100, 20), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::putText(frame, "FPS : " + std::to_string(fps), cv::Point(100, 50), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::imshow("Tracking", frame);

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }

    pedal.shut_down();

    cap.release();
    cv::destroyAllWindows();

    t.join();
    Sleep(2000);
}
