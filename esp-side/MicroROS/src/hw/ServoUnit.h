#ifndef SERVODRIVER_H
#define SERVODRIVER_H

#include <SCServo.h>

#include "common/IHardwareHandler.h"

struct ServoUnitData {
    float tilt_rad;
    float camera_rad;
};

struct ServoUnitFeedback {
    float tilt_rad;
    float camera_rad;
    float tilt_speed;
    float camera_speed;
};

class ServoUnit
    : public IHardwareHandler<ServoUnitData>
    , public IHardwareFeedbackProvider<ServoUnitFeedback> {
private:
    SMS_STS st;

public:  // IWebServiceHandler
    void init() override;
    void onCommandReceive(const ServoUnitData& data) override;
    ServoUnitFeedback getFeedback() override;

private:
    void processServosSet(const float& tilt_angle_rad, const float& camera_angle_rad);
    ServoUnitFeedback processFeedback();
    bool getFeedback(byte servoID);
    int16_t angleToPos(float angle_rad, float radMin, float radMax);
    float clampf(float v, float lo, float hi);
    void setServoAngles(float tiltAngleRad, float cameraAngleRad, uint16_t speed = 1600, uint8_t acc = 100);
    float posToAngle(int pos, float radMin = -3.14159265f, float radMax = 3.14159265f);
};

#endif  // SERVODRIVER_H