#ifndef STREAM_H
#define STREAM_H

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <mutex>
#include <Windows.h>

class Stream {
    cv::VideoCapture cap;
    cv::Mat frame;
    
    int sleepMS;
    bool active;
    bool shut;

    std::mutex frame_mutex;

    void run();

public:
    explicit Stream(int cameraIndex, int sleepMS=20);

    void start();

    void shutDown();

    void read(cv::Mat& frameDest);
};

#endif  // STREAM_H
