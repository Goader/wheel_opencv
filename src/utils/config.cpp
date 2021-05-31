#include "../../include/utils/config.h"

using json = nlohmann::json;

void Config::load() {
    std::ifstream file("../../config.json");
    json j = json::parse(file);

    pedalCameraIndex = j.at("pedalCameraIndex");
    wheelCameraIndex = j.at("wheelCameraIndex");

    pedalTrackingAlgorithm = j.at("pedalTrackingAlgorithm");
    wheelTrackingAlgorithm = j.at("wheelTrackingAlgorithm");
}
