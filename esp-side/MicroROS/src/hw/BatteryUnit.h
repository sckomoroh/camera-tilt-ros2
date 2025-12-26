#pragma once

#include <INA219_WE.h>

class BatteryUnit {
public:
    struct Status {
        float shuntVoltage_mV = 0.0;
        float loadVoltage_V = 0.0;
        float busVoltage_V = 0.0;
        float current_mA = 0.0;
        float power_mW = 0.0;
        bool ina219_overflow = false;
    };

private:
    INA219_WE ina219;

public:
    BatteryUnit();

public:
    void init();

public:
    Status getBatteryStatus();
};