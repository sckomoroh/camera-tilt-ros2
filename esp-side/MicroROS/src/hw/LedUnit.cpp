#include "LedUnit.h"

#include <cstdio>

#include <Arduino.h>

#include <algorithm>

#include "common/Constants.h"
#include "common/udp_log.h"

constexpr const char* FIELD_LED_STATUS = "Status";
constexpr int PWM_MAX = 255;

void LedUnit::onCommandReceive(const LedUnitData& data) {
    auto percentage = std::min<uint8_t>(std::max<uint8_t>(data.lightPercentage, 0), 100);
    setBrightnessPercent(percentage);
    char buffer[256] =  {0};
    snprintf(buffer, 255, "Set light percentage: %d\n", percentage);
    log_udp(buffer);
}

void LedUnit::init() {
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(PWMA, OUTPUT);

    ledcSetup(channel_A, led_freq, ANALOG_WRITE_BITS);
    ledcAttachPin(PWMA, channel_A);
}

uint16_t LedUnit::percentToDuty(uint8_t percent) const {
    return static_cast<uint16_t>((static_cast<uint32_t>(percent) * PWM_MAX) / 100u);
}

void LedUnit::setBrightnessPercent(uint8_t percent) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);

    auto duty = percentToDuty(percent);
    ledcWrite(channel_A, duty);
}
