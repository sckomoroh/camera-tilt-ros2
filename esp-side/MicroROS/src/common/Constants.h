#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

#define NODE_NAME "esp32_node"
#define NODE_NAMESPACE ""

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 32     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define S_RXD 18
#define S_TXD 19

#define S_SCL 33
#define S_SDA 32

#define PWMA 25  // Motor A PWM control
#define AIN2 17  // Motor A input 2
#define AIN1 21  // Motor A input 1
#define channel_A 5
#define led_freq 5000
#define ANALOG_WRITE_BITS 8

// Commands
#define CMD_LED_STATUS 100

#define CMD_OLED_SET_CONTENT 200
#define CMD_OLED_SET_LINE 201
#define CMD_OLED_CLEAR 202

#define CMD_SERVO_SET 300
#define CMD_SERVOS_SET 301
#define CMD_SERVOS_CALIB 302
#define CMD_SERVOS_FEEDBACK 303

#endif  // CONSTANTS_H