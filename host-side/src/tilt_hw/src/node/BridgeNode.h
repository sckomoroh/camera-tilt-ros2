#pragma once

#include <mutex>

#include <rclcpp/rclcpp.hpp>

#include <esp_msgs/msg/servo_command.hpp>
#include <esp_msgs/msg/servo_status.hpp>

class BridgeNode : public rclcpp::Node {
public:
    struct Data {
        float pan = 0.0f;
        float camera = 0.0f;
    };

private:
    rclcpp::Publisher<esp_msgs::msg::ServoCommand>::SharedPtr commandPublisher;
    rclcpp::Subscription<esp_msgs::msg::ServoStatus>::SharedPtr statusSubscription;
    mutable std::mutex statusMutex;
    Data currentStatus;

public:
    BridgeNode();

public:
    void setAngles(float pan, float camera);
    Data getStatus() const;

private:
    void statusCallback(esp_msgs::msg::ServoStatus::Ptr msg);
};