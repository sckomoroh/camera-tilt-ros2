#pragma once

#include <thread>
#include <memory>

#include <hardware_interface/system_interface.hpp>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/executors.hpp>

#include "node/BridgeNode.h"

namespace tilt::hw::interface {
struct Joint {
    enum { TILT = 0, CAMERA = 1, JOINTS_COUNT };

    double position;
    double velocity;

    double position_cmd;

    std::string name;
};

class TiltHWInterface : public hardware_interface::SystemInterface {
private:
    hardware_interface::HardwareComponentInterfaceParams mInfo;
    std::vector<Joint> mJoints;
    std::shared_ptr<BridgeNode> node;
    std::thread nodeThread;
    bool running = false;
    rclcpp::executors::SingleThreadedExecutor executor;

public:
    RCLCPP_SHARED_PTR_DEFINITIONS(TiltHWInterface)

public:
    TiltHWInterface();

public:  // hardware_interface::SystemInterface
    __attribute__((visibility("default"))) hardware_interface::CallbackReturn on_init(
        const hardware_interface::HardwareComponentInterfaceParams& info) override;

    __attribute__((visibility("default"))) std::vector<hardware_interface::StateInterface> export_state_interfaces()
        override;

    __attribute__((visibility("default"))) std::vector<hardware_interface::CommandInterface> export_command_interfaces()
        override;

    __attribute__((visibility("default"))) hardware_interface::CallbackReturn on_configure(
        const rclcpp_lifecycle::State& previous_state) override;

    __attribute__((visibility("default"))) hardware_interface::CallbackReturn on_cleanup(
        const rclcpp_lifecycle::State& previous_state) override;

    __attribute__((visibility("default"))) hardware_interface::CallbackReturn on_activate(
        const rclcpp_lifecycle::State& previous_state) override;

    __attribute__((visibility("default"))) hardware_interface::CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State& previous_state) override;

    __attribute__((visibility("default"))) hardware_interface::return_type read(
        const rclcpp::Time& time,
        const rclcpp::Duration& period) override;

    __attribute__((visibility("default"))) hardware_interface::return_type write(
        const rclcpp::Time& time,
        const rclcpp::Duration& period) override;

private:
    void nodeThreadFunc();
};

}  // namespace tilt::hw::interface