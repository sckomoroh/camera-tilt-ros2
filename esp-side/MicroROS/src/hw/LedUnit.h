#ifndef LEDUNIT_H
#define LEDUNIT_H

#include <cstdint>

#include "common/IHardwareHandler.h"

struct LedUnitData {
    uint8_t lightPercentage;
};

class LedUnit : public IHardwareHandler<LedUnitData> {
public:

public:  // IHardwareHandler
    void onCommandReceive(const LedUnitData& data) override;
    void init() override;

public:
    void setBrightnessPercent(uint8_t percent);

private:
    uint16_t percentToDuty(uint8_t percent) const;
};

#endif  // LEDUNIT_H