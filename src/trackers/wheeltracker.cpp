#include "../../include/trackers/wheeltracker.h"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// angle between (0, -1) and (x, y) vectors
double angle(double x, double y) {
    // base vector is (0, -1), so after scalar product with (x, y) we get only -y
    // posX will instead define the sign (is the wheel turned left or right)
    // in addition, we do not need to divide by the norm of the base vector, as it is already 1
    double cosine = -y / sqrt(x * x + y * y);
    return sgn(x) * acos(cosine) * (180 / M_PI);
}

double WheelTracker::calculateState(double posX, double posY) const {
    // making (0, 0) - the center, won't have to mess with vectors
    posX -= centerX;
    posY -= centerY;

    return angle(posX, posY) * (360 / (maxAngle - minAngle));
}

void WheelTracker::init(std::string trackerTypeParam) {
    if (trackerTypeParam == "CSRT") {
        tracker = cv::legacy::TrackerCSRT::create();
    }
    else if (trackerTypeParam == "MedianFlow") {
        tracker = cv::legacy::TrackerMedianFlow::create();
    }
    else throw std::invalid_argument("The tracker type must be either 'CSRT' or 'MedianFlow'!");

    trackerType = trackerTypeParam;

    cv::Mat frame;

    // calibrate center
    bufferX = bufferY = -1;
    centerX = centerY = -1;
    std::string centerName = "Click on the center of the wheel [" + name + "] (ESC to confirm)";
    cv::namedWindow(centerName, cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback(centerName, WheelTracker::onClick, 0);
    while (true) {
        stream->read(frame);

        cv::imshow(centerName, frame);
        centerX = bufferX;
        centerY = bufferY;

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }
    cv::destroyWindow(centerName);

    // check if center is set
    if (centerX == -1 || centerY == -1) {
        throw std::runtime_error("You have not clicked on the center of the wheel!");
    }

    // calibrate minAngle and maxAngle
    bufferX = -1;
    bufferY = -1;
    std::string angleName = "Click on the fully turned wheel [" + name + "] (ESC to confirm)";
    cv::namedWindow(angleName, cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback(angleName, WheelTracker::onClick, 0);
    while (true) {
        stream->read(frame);

        cv::imshow(angleName, frame);
        maxAngle = abs(angle(bufferX - centerX, bufferY - centerY));
        minAngle = -maxAngle;

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }
    cv::destroyWindow(angleName);

    std::string ROIName = "Select ROI [" + name + "] (ENTER to confirm)";
    cv::Rect2d bbox = cv::selectROI(ROIName, frame, true);
    tracker->init(frame, bbox);
    cv::destroyWindow(ROIName);
}
