#include "Node.h"

#include <algorithm>
#include <cstdio>

#include "common/udp_log.h"
#include "common/Constants.h"

NodeContext nodeGlobalContext;

static void publisherTimerCallback(rcl_timer_t* timer, int64_t last_call_time);
static void servoCommandCallback(const void* msgin);
static void flashServiceCallback(const void* request, void* response);
static void oledMgmtServiceCallback(const void* request, void* response);

#define RCLC_CHECK(fn)                           \
    do {                                         \
        rcl_ret_t _rc = (fn);                    \
        if (_rc != RCL_RET_OK) {                 \
            log_udp(#fn);                        \
            log_udp(" ");                        \
            char buffer[32] = {0};               \
            snprintf(buffer, 31, "C: %d ", _rc); \
            log_udp(buffer);                     \
            log_udp("\n");                       \
            rcl_reset_error();                   \
            return;                              \
        }                                        \
    } while (0);

Node::Node() { nodeGlobalContext.node = this; }

void Node::setup() {
    set_microros_serial_transports(Serial);

    allocator = rcl_get_default_allocator();

    RCLC_CHECK(rclc_support_init(&support, 0, nullptr, &allocator));

    RCLC_CHECK(rclc_node_init_default(&node, NODE_NAME, NODE_NAMESPACE, &support));

    RCLC_CHECK(rclc_publisher_init_default(&servoStatusPublisher, &node,
                                           ROSIDL_GET_MSG_TYPE_SUPPORT(esp_msgs, msg, ServoStatus), "servo_status"));

    RCLC_CHECK(rclc_subscription_init_default(
        &servoCommandSubscriber, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(esp_msgs, msg, ServoCommand), "servo_command"));

    RCLC_CHECK(rclc_service_init_default(&flashService, &node,
                                         ROSIDL_GET_SRV_TYPE_SUPPORT(esp_msgs, srv, SetFlashlight), "set_flashlight"));

    RCLC_CHECK(rclc_executor_init(&executor, &support.context, 5, &allocator));

    const unsigned int timer_timeout = 10;
    RCLC_CHECK(rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(timer_timeout), publisherTimerCallback));

    RCLC_CHECK(rclc_executor_add_subscription(&executor, &servoCommandSubscriber, &command_msg, servoCommandCallback,
                                              ON_NEW_DATA));

    RCLC_CHECK(
        rclc_executor_add_service(&executor, &flashService, &flashSrvRequest, &flashSrvResponse, flashServiceCallback));

    RCLC_CHECK(rclc_executor_add_timer(&executor, &timer));
}

void Node::spin() {
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
}

void publisherTimerCallback(rcl_timer_t* timer, int64_t last_call_time) {
    (void)last_call_time;
    if (!timer)
        return;

    if (nodeGlobalContext.servoUnitFeedback != nullptr) {
        ServoUnitFeedback feedback = nodeGlobalContext.servoUnitFeedback->getFeedback();

        nodeGlobalContext.node->servoStatusMsg.camera_rad = feedback.camera_rad;
        nodeGlobalContext.node->servoStatusMsg.tilt_rad = feedback.tilt_rad;
    }

    RCLC_CHECK(
        rcl_publish(&nodeGlobalContext.node->servoStatusPublisher, &nodeGlobalContext.node->servoStatusMsg, nullptr));
}

void servoCommandCallback(const void* msgin) {
    const esp_msgs__msg__ServoCommand* msg = (const esp_msgs__msg__ServoCommand*)msgin;

    if (nodeGlobalContext.servoUnit != nullptr) {
        ServoUnitData servoUnitData;
        servoUnitData.tilt_rad = msg->tilt_rad;
        servoUnitData.camera_rad = msg->camera_rad;
        nodeGlobalContext.servoUnit->onCommandReceive(servoUnitData);
    }
}

void flashServiceCallback(const void* request, void* response) {
    const esp_msgs__srv__SetFlashlight_Request* req = (const esp_msgs__srv__SetFlashlight_Request*)request;
    esp_msgs__srv__SetFlashlight_Response* res = (esp_msgs__srv__SetFlashlight_Response*)response;

    if (nodeGlobalContext.ledUnit != nullptr) {
        LedUnitData ledUnitData;
        ledUnitData.lightPercentage = req->percent;
        nodeGlobalContext.ledUnit->onCommandReceive(ledUnitData);
        res->success = true;
    } else {
        res->success = false;
    }
}
