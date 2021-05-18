#include "../../include/trackers/pedaltracker.h"

int PedalTracker::bufferY = -1;

void PedalTracker::onClick(int event, int x, int y, int flags, void* param) {
    if (event != cv::EVENT_LBUTTONDOWN) return;
    PedalTracker::bufferY = y;
}

void PedalTracker::init(std::string trackerType) {
    if (trackerType == "CSRT") {
        tracker = cv::legacy::TrackerCSRT::create();
    }
    else if (trackerType == "MedianFlow") {
        tracker = cv::legacy::TrackerMedianFlow::create();
    }
    else throw std::invalid_argument("The tracker type must be either 'CSRT' or 'MedianFlow'!");

    this->trackerType = trackerType;

    cv::Mat frame;
    bool ok;

    // calibrate maxY
    bufferY = -1;
    std::string highestPosName = "Click on the highest position [" + name + "]";
    cv::namedWindow(highestPosName, cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback(highestPosName, PedalTracker::onClick, 0);
    while (true) {
        ok = cap->read(frame);
        if (!ok) throw std::system_error(ENODEV, 
                                         std::generic_category(), 
                                         "Frame cannot be read from device");

        cv::imshow(highestPosName, frame);
        maxY = bufferY;
        std::cout << "DEBUG: " << maxY << std::endl;

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }
    cv::destroyWindow(highestPosName);

    // calibrate minY
    bufferY = -1;
    std::string lowestPosName = "Click on the lowest position [" + name + "]";
    cv::namedWindow(lowestPosName, cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback(lowestPosName, PedalTracker::onClick, 0);
    while (true) {
        ok = cap->read(frame);
        if (!ok) throw std::system_error(ENODEV, 
                                         std::generic_category(), 
                                         "Frame cannot be read from device");

        cv::imshow(lowestPosName, frame);
        minY = bufferY;
        std::cout << "DEBUG: " << minY << std::endl;

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }
    cv::destroyWindow(lowestPosName);

    // check if both minY and maxY are set
    if (minY == -1 || maxY == -1) {
        throw std::runtime_error("You have not clicked in one of the windows!");
    }

    // ensure that minY and maxY have different values
    if (minY == maxY) {
        throw std::runtime_error("Highest and lowest position must have different values!");
    }

    // select ROI and init tracker
    for (int i = 0; i < 10; i++) {
        ok = cap->read(frame);
        if (!ok)
            throw std::system_error(ENODEV, std::generic_category(), "Frame cannot be read from device");
    }

    std::string ROIName = "Select ROI [" + name + "]";
    cv::Rect2d bbox = cv::selectROI(ROIName, frame, true);
    tracker->init(frame, bbox);
    cv::destroyWindow(ROIName);
}

void PedalTracker::track() const {
    cv::Mat frame;
    cv::Rect2d bbox;
    bool ok;

    std::string windowName = "Tracking [" + name + "]";
    cv::namedWindow(windowName, cv::WindowFlags::WINDOW_AUTOSIZE);

    while (true) {
        ok = cap->read(frame);
        if (!ok) break;
        
        int64 timer = cv::getTickCount();

        ok = tracker->update(frame, bbox);

        // sets value depending on the current position and calibrated bounds
        double position = bbox.y + bbox.height / 2;
        pedal->setPressed((position - minY) / (maxY - minY));

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
