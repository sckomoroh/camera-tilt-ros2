from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command, PathJoinSubstitution, FindExecutable
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    frame_provider = Node(
        package='camera_rtsp_source',
        executable='frame_provider',
        output='screen',
    )

    return LaunchDescription([

        frame_provider,
    ])
