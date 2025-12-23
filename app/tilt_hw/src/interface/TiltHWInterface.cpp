#include "TiltHWInterface.h"

#include "hardware_interface/types/hardware_interface_type_values.hpp"

#define TILT_JOINT_NAME "tilt_joint"
#define CAMERA_JOINT_NAME "camera_joint"

using hardware_interface::CallbackReturn;
using rclcpp::get_logger;

namespace tilt::hw::interface
{

    TiltHWInterface::TiltHWInterface() { mJoints.resize(2); }

    hardware_interface::CallbackReturn TiltHWInterface::on_init(
        const hardware_interface::HardwareInfo &info)
    {
        if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
        {
            RCLCPP_ERROR(get_logger("tilt_hw"), "SystemInterface::on_init failed");
            return CallbackReturn::ERROR;
        }

        mInfo = {info};
        RCLCPP_INFO(get_logger("tilt_hw"), "on_init");

        mJoints[Joint::TILT].name = mInfo.hardware_parameters[TILT_JOINT_NAME];
        mJoints[Joint::CAMERA].name = mInfo.hardware_parameters[CAMERA_JOINT_NAME];

        RCLCPP_INFO(get_logger("tilt_hw"), "on_init: Tile '%s' Pan '%s'", mJoints[Joint::TILT].name.c_str(),
                    mJoints[Joint::CAMERA].name.c_str());

        return CallbackReturn::SUCCESS;
    }

    std::vector<hardware_interface::StateInterface> TiltHWInterface::export_state_interfaces()
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "export_state_interfaces.");
        std::vector<hardware_interface::StateInterface> state_interfaces;

        state_interfaces.emplace_back(hardware_interface::StateInterface(
            mJoints[Joint::TILT].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::TILT].position));
        state_interfaces.emplace_back(hardware_interface::StateInterface(
            mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::CAMERA].position));

        // state_interfaces.emplace_back(hardware_interface::StateInterface(
        //     mJoints[Joint::TILT].name, hardware_interface::HW_IF_VELOCITY, &mJoints[Joint::TILT].velocity));
        // state_interfaces.emplace_back(hardware_interface::StateInterface(
        //     mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_VELOCITY, &mJoints[Joint::CAMERA].velocity));

        return state_interfaces;
    }

    std::vector<hardware_interface::CommandInterface> TiltHWInterface::export_command_interfaces()
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "export_command_interfaces");

        std::vector<hardware_interface::CommandInterface> command_interfaces;
        command_interfaces.emplace_back(hardware_interface::CommandInterface(
            mJoints[Joint::TILT].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::TILT].position_cmd));
        command_interfaces.emplace_back(hardware_interface::CommandInterface(
            mJoints[Joint::CAMERA].name, hardware_interface::HW_IF_POSITION, &mJoints[Joint::CAMERA].position_cmd));

        return command_interfaces;
    }

    hardware_interface::CallbackReturn TiltHWInterface::on_configure(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "on_configure: Previos state: '%s'", previous_state.label().c_str());

        return CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn TiltHWInterface::on_cleanup(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "on_cleanup: Previos state: '%s'", previous_state.label().c_str());

        return CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn TiltHWInterface::on_activate(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "on_activate. Previos state: '%s'", previous_state.label().c_str());

        return CallbackReturn::SUCCESS;
    }

    hardware_interface::CallbackReturn TiltHWInterface::on_deactivate(const rclcpp_lifecycle::State &previous_state)
    {
        RCLCPP_INFO(get_logger("tilt_hw"), "on_deactivate: Previos state: '%s'", previous_state.label().c_str());

        return CallbackReturn::SUCCESS;
    }

    hardware_interface::return_type TiltHWInterface::read(const rclcpp::Time & /*time*/,
                                                          const rclcpp::Duration & /*period*/)
    {
        driver.getStates(mJoints[Joint::TILT].position, mJoints[Joint::CAMERA].position, mJoints[Joint::TILT].velocity,
                         mJoints[Joint::CAMERA].velocity);

        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type TiltHWInterface::write(const rclcpp::Time & /*time*/,
                                                           const rclcpp::Duration & /*period*/)
    {
        if (mJoints[Joint::TILT].position_cmd > 0.001 || mJoints[Joint::TILT].position_cmd < -0.001 ||
            mJoints[Joint::CAMERA].position_cmd > 0.001 || mJoints[Joint::CAMERA].position_cmd < -0.001)
        {
            RCLCPP_INFO(get_logger("tilt_hw"), "write: Tilt=%.3f Camera=%.3f", mJoints[Joint::TILT].position_cmd,
                        mJoints[Joint::CAMERA].position_cmd);
        }

        driver.sendAngles(mJoints[Joint::TILT].position_cmd, mJoints[Joint::CAMERA].position_cmd);
        return hardware_interface::return_type::OK;
    }

} // namespace tilt::hw::interface

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(tilt::hw::interface::TiltHWInterface, hardware_interface::SystemInterface)