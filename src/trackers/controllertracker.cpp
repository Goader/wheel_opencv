#include "../../include/trackers/controllertracker.h"

int ControllerTracker::bufferX = -1;
int ControllerTracker::bufferY = -1;

void ControllerTracker::onClick(int event, int x, int y, int, void*) {
    if (event != cv::EVENT_LBUTTONDOWN) return;
    ControllerTracker::bufferX = x;
    ControllerTracker::bufferY = y;
}

void ControllerTracker::track() const {
    cv::Mat frame;
    cv::Rect2d bbox;
    bool ok;

    std::string windowName = "Tracking [" + name + "] (ESC to exit)";
    cv::namedWindow(windowName, cv::WindowFlags::WINDOW_AUTOSIZE);

    while (true) {
        stream->read(frame);
        
        int64 timer = cv::getTickCount();

        ok = tracker->update(frame, bbox);

        // sets value depending on the current position and calibrated bounds
        double posX = bbox.x + bbox.width / 2;
        double posY = bbox.y + bbox.height / 2;
        controller->setState(calculateState(posX, posY));

        auto fps = cv::getTickFrequency() / (cv::getTickCount() - timer);

        // showing bbox
        if (ok)
            cv::rectangle(frame, bbox, cv::Scalar(0, 255, 0), 2, 1);
        else 
            cv::putText(frame, "Tracking failure detected", cv::Point(100, 80), 
                        cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);

        // legend
        cv::putText(frame, trackerType + " Tracker", cv::Point(100, 20), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::putText(frame, "FPS : " + std::to_string(fps), cv::Point(100, 50), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

        cv::imshow(windowName, frame);

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }

    cv::destroyWindow(windowName);
}
