from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import FindExecutable


def generate_launch_description():
    pkg = FindPackageShare('camera_description')

    xacro_file = PathJoinSubstitution([pkg, 'urdf', 'part.urdf'])

    robot_description = ParameterValue(
        Command([FindExecutable(name='xacro'), ' ', xacro_file]),
        value_type=str
    )

    print(f'Robot description: {robot_description}')

    return LaunchDescription([
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': robot_description}],
        ),
    ])
