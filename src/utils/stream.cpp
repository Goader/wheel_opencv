#include "../../include/utils/stream.h"

Stream::Stream(int cameraIndex, int sleepMS) : 
        cap{cameraIndex}, frame{}, sleepMS{sleepMS}, frame_mutex{}, shut{false} {
    if (!cap.isOpened()) {
        throw std::system_error(ENODEV, std::generic_category(), "Cannot access the device");
    }
}

void Stream::start() {
    if (shut) {
        throw std::runtime_error("You cannot start the stream which has been already shut down!");
    }
    active = true;
    run();
}

void Stream::run() {
    bool ok;
    while (active) {
        frame_mutex.lock();
        ok = cap.read(frame);
        if (!ok) {
            throw std::system_error(ENODEV, std::generic_category(), "Frame cannot be read from device");
        }
        frame_mutex.unlock();
        Sleep(sleepMS);
    }
}

void Stream::read(cv::Mat& frame) {
    frame_mutex.lock();
    frame = this->frame.clone();  // FIXME not sure if works ok with deep copy
    frame_mutex.unlock();
}

void Stream::shut_down() {
    shut = true;
    active = false;
    cap.release();
}
