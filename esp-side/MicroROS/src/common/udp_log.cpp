#include "udp_log.h"

#include <WiFi.h>
#include <WiFiUdp.h>

static WiFiUDP udp;
static IPAddress logHost(192, 168, 0, 106);  // IP твоего хоста
static const uint16_t logPort = 15150;

void log_udp(const char* s) {
    udp.beginPacket(logHost, logPort);
    udp.write((const uint8_t*)s, strlen(s));
    udp.endPacket();
    udp.flush();
}
