#include "TiltHWInterface.h"

#define TILT_JOINT_NAME "tilt_joint"
#define CAMERA_JOINT_NAME "camera_joint"

using hardware_interface::CallbackReturn;
using rclcpp::get_logger;

namespace tilt::hw::interface {

TiltHWInterface::TiltHWInterface() { 
    fprintf(stderr, "[DEBUG] Create ROS2 BridgeNode\n");
    node = std::make_shared<BridgeNode>();

    mJoints.resize(2); }

hardware_interface::CallbackReturn TiltHWInterface::on_init(
    const hardware_interface::HardwareComponentInterfaceParams& info) {
    if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {
        RCLCPP_ERROR(get_logger(), "SystemInterface::on_init failed");
        return CallbackReturn::ERROR;
    }

    mInfo = {info};
    RCLCPP_INFO(get_logger(), "on_init");

    mJoints[Joint::TILT].name = mInfo.hardware_info.hardware_parameters[TILT_JOINT_NAME];
    mJoints[Joint::CAMERA].name = mInfo.hardware_info.hardware_parameters[CAMERA_JOINT_NAME];

    RCLCPP_INFO(get_logger(), "on_init: Tile '%s' Camera '%s'", mJoints[Joint::TILT].name.c_str(),
                mJoints[Joint::CAMERA].name.c_str());

    executor.add_node(node->get_node_base_interface());

    return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> TiltHWInterface::export_state_interfaces() {
    RCLCPP_INFO(get_logger(), "export_state_interfaces.");
    std::vector<hardware_interface::StateInterface> state_interfaces;

    state_interfaces.emplace_back(hardware_interface::StateInterface(
        mJoints[Joint::TILT].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::TILT].position));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::CAMERA].position));

    state_interfaces.emplace_back(hardware_interface::StateInterface(
        mJoints[Joint::TILT].name, hardware_interface::HW_IF_VELOCITY, &mJoints[Joint::TILT].velocity));
    state_interfaces.emplace_back(hardware_interface::StateInterface(
        mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_VELOCITY, &mJoints[Joint::CAMERA].velocity));

    return state_interfaces;
}

std::vector<hardware_interface::CommandInterface> TiltHWInterface::export_command_interfaces() {
    RCLCPP_INFO(get_logger(), "export_command_interfaces");

    std::vector<hardware_interface::CommandInterface> command_interfaces;
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        mJoints[Joint::TILT].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::TILT].position_cmd));
    command_interfaces.emplace_back(hardware_interface::CommandInterface(
        mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::CAMERA].position_cmd));

    return command_interfaces;
}

hardware_interface::CallbackReturn TiltHWInterface::on_configure(const rclcpp_lifecycle::State& previous_state) {
    RCLCPP_INFO(get_logger(), "on_configure: Previos state: '%s'", previous_state.label().c_str());

    return CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn TiltHWInterface::on_cleanup(const rclcpp_lifecycle::State& previous_state) {
    RCLCPP_INFO(get_logger(), "on_cleanup: Previos state: '%s'", previous_state.label().c_str());

    return CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn TiltHWInterface::on_activate(const rclcpp_lifecycle::State& previous_state) {
    RCLCPP_INFO(get_logger(), "on_activate. Previos state: '%s'", previous_state.label().c_str());

    running = true;
    nodeThread = std::thread(&TiltHWInterface::nodeThreadFunc, this);

    return CallbackReturn::SUCCESS;
}

hardware_interface::CallbackReturn TiltHWInterface::on_deactivate(const rclcpp_lifecycle::State& previous_state) {
    RCLCPP_INFO(get_logger(), "on_deactivate: Previos state: '%s'", previous_state.label().c_str());

    running = false;
    if (nodeThread.joinable()) {
        nodeThread.join();
    }

    return CallbackReturn::SUCCESS;
}

hardware_interface::return_type TiltHWInterface::read(const rclcpp::Time& /*time*/,
                                                      const rclcpp::Duration& /*period*/) {
    auto status = node->getStatus();
    mJoints[Joint::TILT].position = static_cast<double>(status.pan);
    mJoints[Joint::CAMERA].position = static_cast<double>(status.camera);
    return hardware_interface::return_type::OK;
}

hardware_interface::return_type TiltHWInterface::write(const rclcpp::Time& /*time*/,
                                                       const rclcpp::Duration& /*period*/) {
    node->setAngles(static_cast<float>(mJoints[Joint::TILT].position_cmd),
                    static_cast<float>(mJoints[Joint::CAMERA].position_cmd));

    return hardware_interface::return_type::OK;
}

void TiltHWInterface::nodeThreadFunc() {
    while (running == true) {
        executor.spin_once(std::chrono::milliseconds(100));
    }
}

}  // namespace tilt::hw::interface

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(tilt::hw::interface::TiltHWInterface, hardware_interface::SystemInterface)