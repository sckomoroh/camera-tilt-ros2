#pragma once

#include <curl/curl.h>
#include <string>
#include <stdexcept>

class TiltDriver {
public:
    void sendAngles(double tiltAngle, double cameraAngle);
    void getStates(double& tiltAngle, double& cameraAngle, double& tiltVelocity, double& cameraVelocity);

private:
    std::string httpPostJson(const std::string& url, const std::string& json_body, long timeout_ms = 500);
};
