#include "BridgeNode.h"

BridgeNode::BridgeNode()
    : rclcpp::Node{"hw_bridge_node"} {
    RCLCPP_INFO(get_logger(), "Create Node");
    commandPublisher = create_publisher<esp_msgs::msg::ServoCommand>("/servo_command", 10);
    statusSubscription = create_subscription<esp_msgs::msg::ServoStatus>(
        "/servo_status", 10, std::bind(&BridgeNode::statusCallback, this, ::std::placeholders::_1));
}

void BridgeNode::statusCallback(esp_msgs::msg::ServoStatus::Ptr msg) {
    std::lock_guard<std::mutex> lock(statusMutex);

    currentStatus.pan = msg->tilt_rad;
    currentStatus.camera = msg->camera_rad;
}

BridgeNode::Data BridgeNode::getStatus() const {
    std::lock_guard<std::mutex> lock(statusMutex);
    Data data = currentStatus;
    return data;
}

void BridgeNode::setAngles(float tilt, float camera) {
    esp_msgs::msg::ServoCommand msg;
    msg.tilt_rad = tilt;
    msg.camera_rad = camera;
    commandPublisher->publish(msg);
}