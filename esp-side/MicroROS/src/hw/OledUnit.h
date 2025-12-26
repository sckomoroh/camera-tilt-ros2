#ifndef OLEDUNIT_H
#define OLEDUNIT_H

#include <cstdint>

#include <array>
#include <vector>

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class OledUnit {
private:
    Adafruit_SSD1306 display;
    std::array<String, 4> content;

public:
    OledUnit();

public:
    void init();

    void processClear();
    void setContent(const std::vector<String>& content);

    void setLine(uint8_t lineNumber, const String& line);

private:
    void clear();
    void updateDisplay();
};

#endif  // OLEDUNIT_H