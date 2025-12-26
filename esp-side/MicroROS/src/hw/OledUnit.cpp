#include "OledUnit.h"

#include <cstdio>

#include "common/Constants.h"
#include "common/udp_log.h"

OledUnit::OledUnit()
    : display{SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET} {}

void OledUnit::init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        log_udp("[ERROR] Failed to setup OLED unit\n");
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.display();
}

void OledUnit::processClear() { clear(); }

void OledUnit::setContent(const std::vector<String>& content) {
    if (content.size() != 4) {
        log_udp("[ERROR] Content size mismatch\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        this->content[i] = content[i];
    }

    updateDisplay();
}

void OledUnit::setLine(uint8_t lineNumber, const String& line) {
    if (lineNumber < 0 || lineNumber > 3) {
        log_udp("[ERROR] Index out of range\n");
        return;
    }

    content[lineNumber] = line;

    updateDisplay();
}

void OledUnit::clear() {
    for (int i = 0; i < 4; i++) {
        content[i] = "";
    }

    updateDisplay();
}

void OledUnit::updateDisplay() {
    display.clearDisplay();
    display.setCursor(0, 0);

    for (int i = 0; i < 4; i++) {
        display.println(content[i].c_str());
    }

    display.display();
}
