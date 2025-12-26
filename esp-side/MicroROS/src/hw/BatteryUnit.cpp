#include "BatteryUnit.h"



#include "common/Constants.h"

#define INA219_ADDRESS 0x42

BatteryUnit::BatteryUnit()
    : ina219{INA219_ADDRESS} {}

void BatteryUnit::init() {
    if (!ina219.init()) {
        Serial.println("INA219 not connected!");
        return;
    }

    ina219.setADCMode(ina219AdcMode::INA219_BIT_MODE_9);
    ina219.setPGain(ina219PGain::INA219_PG_320);
    ina219.setBusRange(ina219BusRange::INA219_BRNG_16);
    ina219.setShuntSizeInOhms(0.01);  // used in INA219.
}

BatteryUnit::Status BatteryUnit::getBatteryStatus() {
    Status status;
    status.shuntVoltage_mV = ina219.getShuntVoltage_mV();
    status.busVoltage_V = ina219.getBusVoltage_V();
    status.current_mA = ina219.getCurrent_mA();
    status.power_mW = ina219.getBusPower();
    status.loadVoltage_V = status.busVoltage_V + (status.shuntVoltage_mV / 1000);
    status.ina219_overflow = ina219.getOverflow();

    // Serial.printf("Load:  %.02fV Power: %.02fmW Bus: %.02fV\n", status.loadVoltage_V, status.power_mW,
    //               status.busVoltage_V);

    return status;
}