// #include <Arduino.h>

#include <Arduino.h>
#include <WiFi.h>

#include <cstdio>
#include <cstring>

#include "common/Constants.h"
#include "hw/BatteryUnit.h"
#include "ros2/Node.h"

LedUnit* ledUnit = nullptr;
ServoUnit* servoUnit = nullptr;
OledUnit* oledUnit = nullptr;
BatteryUnit* batteryUnit = nullptr;

Node* node = nullptr;
BatteryUnit::Status batteryStatus;

const char* SSID = "cplusplus";
const char* PASS = "Anna198800";

#define LINE_SIZE 256

char line[LINE_SIZE];

void setup() {
    Serial.begin(115200);
    delay(500);

    Wire.begin(S_SDA, S_SCL);
    Wire.setClock(400000);

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.begin(SSID, PASS);
    uint32_t t0 = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        if (millis() - t0 > 15000) {  // 15s
            break;
        }
    }

    oledUnit = new OledUnit();
    ledUnit = new LedUnit();
    servoUnit = new ServoUnit();
    batteryUnit = new BatteryUnit();

    nodeGlobalContext.ledUnit = ledUnit;
    nodeGlobalContext.servoUnit = servoUnit;
    nodeGlobalContext.servoUnitFeedback = servoUnit;

    node = new Node();

    oledUnit->init();
    ledUnit->init();
    servoUnit->init();
    node->setup();
}

static void updateOledInfo();

int cp = 0;

void loop() {
    node->spin();
    updateOledInfo();

    delay(10);
}

void updateOledInfo() {
    memset(line, 0, LINE_SIZE);
    batteryStatus = batteryUnit->getBatteryStatus();
    snprintf(line, LINE_SIZE - 1, "Battery: %.01fV", batteryStatus.busVoltage_V);
    oledUnit->setLine(0, line);
}
