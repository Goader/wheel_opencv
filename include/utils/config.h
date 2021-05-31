#ifndef CONFIG_H
#define CONFIG_H

// https://github.com/nlohmann/json
#include "json.hpp"
#include <fstream>

class Config {
    int pedalCameraIndex;
    int wheelCameraIndex;

    std::string pedalTrackingAlgorithm;
    std::string wheelTrackingAlgorithm;
public:
    Config() : pedalCameraIndex{0},
               wheelCameraIndex{0},
               pedalTrackingAlgorithm{},
               wheelTrackingAlgorithm{} {}

    void load();

    int getPedalCameraIndex() const {
        return pedalCameraIndex;
    }

    int getWheelCameraIndex() const {
        return wheelCameraIndex;
    }

    std::string getPedalTrackingAlgorithm() const {
        return pedalTrackingAlgorithm;
    }

    std::string getWheelTrackingAlgorithm() const {
        return wheelTrackingAlgorithm;
    }
};

#endif  // CONFIG_H
