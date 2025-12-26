#include "ServoUnit.h"

#include <numeric>

#include "common/Constants.h"

#define GIMBAL_TILT_ID 2    // 360
#define GIMBAL_CAMERA_ID 1  // Vertical move

static constexpr float TILT_MIN = -M_PI;    // -180 degrees
static constexpr float TILT_MAX = +M_PI;    // +180 degrees
static constexpr float CAMERA_MIN = -M_PI_2;  // -90 degrees
static constexpr float CAMERA_MAX = 0.52359878f;   // +30 degrees

struct ServoFeedback {
    bool status;
    int pos;
    int speed;
    int load;
    float voltage;
    float current;
    float temper;
    byte mode;
};

ServoFeedback servoFeedback[2];

void ServoUnit::onCommandReceive(const ServoUnitData& data) { processServosSet(data.tilt_rad, data.camera_rad); }

ServoUnitFeedback ServoUnit::getFeedback() { return processFeedback(); }

ServoUnitFeedback ServoUnit::processFeedback() {
    ServoUnitFeedback feedback;

    if (st.FeedBack(GIMBAL_CAMERA_ID) != -1) {
        auto pos = st.ReadPos(-1);
        feedback.camera_rad = posToAngle(pos);
        feedback.camera_speed = st.ReadSpeed(-1);
    }

    if (st.FeedBack(GIMBAL_TILT_ID) != -1) {
        auto pos = st.ReadPos(-1);
        feedback.tilt_rad = posToAngle(pos);
        feedback.tilt_speed = st.ReadSpeed(-1);
    }

    return feedback;
}

void ServoUnit::processServosSet(const float& tilt_angle_rad, const float& camera_angle_rad) {
    setServoAngles(camera_angle_rad, tilt_angle_rad);
}

void ServoUnit::init() {
    Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
    st.pSerial = &Serial1;
    while (!Serial1) {
    }

    auto servo1 = getFeedback(GIMBAL_TILT_ID);
    auto servo2 = getFeedback(GIMBAL_CAMERA_ID);
    auto err = st.CalibrationOfs(GIMBAL_TILT_ID);
    err = st.CalibrationOfs(GIMBAL_CAMERA_ID);
}

bool ServoUnit::getFeedback(byte servoID) {
    if (st.FeedBack(servoID) != -1) {
        servoFeedback[servoID - 1].status = true;
        servoFeedback[servoID - 1].pos = st.ReadPos(-1);
        servoFeedback[servoID - 1].speed = st.ReadSpeed(-1);
        servoFeedback[servoID - 1].load = st.ReadLoad(-1);
        servoFeedback[servoID - 1].voltage = st.ReadVoltage(-1);
        servoFeedback[servoID - 1].current = st.ReadCurrent(-1);
        servoFeedback[servoID - 1].temper = st.ReadTemper(-1);
        servoFeedback[servoID - 1].mode = st.ReadMode(servoID);
        return true;
    } else {
        servoFeedback[servoID - 1].status = false;
        return false;
    }
}

int16_t ServoUnit::angleToPos(float angle_rad, float radMin, float radMax) {
    angle_rad = clampf(angle_rad, radMin, radMax);
    float norm = (angle_rad + M_PI) / M_TWOPI;
    return (int)lroundf(norm * 4095.0f);
}

float ServoUnit::clampf(float v, float lo, float hi) {
    if (v < lo) {
        return lo;
    }

    if (v > hi) {
        return hi;
    }

    return v;
}

float ServoUnit::posToAngle(int pos, float radMin, float radMax) {
    pos = std::min(std::max(pos, 0), 4095);
    float norm = (float)pos / 4095.0f;  // 0..1
    float angle = norm * (radMax - radMin) + radMin;
    return angle;
}

void ServoUnit::setServoAngles(float tiltAngleRad, float cameraAngleRad, uint16_t speed, uint8_t acc) {
    const int16_t cameraPos = angleToPos(cameraAngleRad, CAMERA_MIN, CAMERA_MAX);
    const int16_t tiltPos = angleToPos(tiltAngleRad, TILT_MIN, TILT_MAX);

    uint8_t ids[2] = {GIMBAL_TILT_ID, GIMBAL_CAMERA_ID};
    short posA[2] = {tiltPos, cameraPos};
    uint16_t spdA[2] = {speed, speed};
    uint8_t accA[2] = {acc, acc};
    st.SyncWritePosEx(ids, 2, posA, spdA, accA);
}