#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/executor.h>
#include <rclc/rclc.h>

#include <esp_msgs/msg/servo_command.h>
#include <esp_msgs/msg/servo_status.h>
#include <esp_msgs/srv/oled_mgmt.h>
#include <esp_msgs/srv/set_flashlight.h>

#include "common/IHardwareHandler.h"
#include "hw/LedUnit.h"
#include "hw/OledUnit.h"
#include "hw/ServoUnit.h"

class Node;

struct NodeContext {
    IHardwareHandler<LedUnitData>* ledUnit = nullptr;
    IHardwareHandler<ServoUnitData>* servoUnit = nullptr;
    IHardwareFeedbackProvider<ServoUnitFeedback>* servoUnitFeedback = nullptr;
    Node* node = nullptr;
};

extern NodeContext nodeGlobalContext;

class Node {
private:
    rcl_subscription_t servoCommandSubscriber;
    rcl_service_t flashService;
    rcl_service_t oledService;
    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    rcl_timer_t timer;
    esp_msgs__msg__ServoCommand command_msg;
    esp_msgs__srv__SetFlashlight_Request flashSrvRequest;
    esp_msgs__srv__SetFlashlight_Response flashSrvResponse;
    esp_msgs__srv__OledMgmt_Request oledMgmtRequest;
    esp_msgs__srv__OledMgmt_Response oledMgmtResponse;

public:
    esp_msgs__msg__ServoStatus servoStatusMsg;
    rcl_publisher_t servoStatusPublisher;

public:
    Node();

public:
    void setup();
    void spin();

private:
};