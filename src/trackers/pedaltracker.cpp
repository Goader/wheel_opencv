#include "../../include/trackers/pedaltracker.h"

double PedalTracker::calculateState(double posX, double posY) const {
    return 100 * (posY - minY) / (maxY - minY);
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

    // calibrate maxY
    bufferY = -1;
    std::string highestPosName = "Click on the highest position [" + name + "]";
    cv::namedWindow(highestPosName, cv::WindowFlags::WINDOW_AUTOSIZE);
    cv::setMouseCallback(highestPosName, PedalTracker::onClick, 0);
    while (true) {
        stream->read(frame);

        cv::imshow(highestPosName, frame);
        minY = bufferY;
        std::cout << "DEBUG: " << minY << std::endl;

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
        stream->read(frame);

        cv::imshow(lowestPosName, frame);
        maxY = bufferY;
        std::cout << "DEBUG: " << maxY << std::endl;

        int k = cv::waitKey(1) & 0xff;  // ESC button
        if (k == 27) break;
    }
    cv::destroyWindow(lowestPosName);

    // check if both minY and maxY are set
    if (minY == -1 || maxY == -1) {
        throw std::runtime_error("You have not clicked in one of the windows!");
    }

    // ensure that minY and maxY have different values
    if (maxY <= minY) {
        throw std::runtime_error("The highest position must have greater value than the lowest!");
    }

    std::string ROIName = "Select ROI [" + name + "]";
    cv::Rect2d bbox = cv::selectROI(ROIName, frame, true);
    tracker->init(frame, bbox);
    cv::destroyWindow(ROIName);
}
