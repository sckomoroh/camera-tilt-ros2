from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command, PathJoinSubstitution, FindExecutable
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    description_file = LaunchConfiguration('description_file')
    controllers_pkg = LaunchConfiguration('controllers_package')
    controllers_file = LaunchConfiguration('controllers_file')
    use_sim_time = LaunchConfiguration('use_sim_time')
    controller_name = LaunchConfiguration('controller_name')

    robot_description = ParameterValue(
        Command([
            FindExecutable(name='xacro'), ' ',
            PathJoinSubstitution([FindPackageShare(controllers_pkg), 'urdf', description_file])
        ]),
        value_type=str
    )

    frame_provider = Node(
        package='nn_node',
        executable='nn_node',
        output='screen',
    )

    rsp = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description,
                     'use_sim_time': use_sim_time,
                     'log_level': LaunchConfiguration('log_level')}],
    )

    control_node = Node(
        package='controller_manager',
        executable='ros2_control_node',
        output='screen',
        parameters=[
            {'log_level': LaunchConfiguration('log_level')},
            {'robot_description': robot_description,
             'use_sim_time': use_sim_time},
            PathJoinSubstitution([FindPackageShare(controllers_pkg), 'controllers', controllers_file]),
        ],
    )

    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        output='screen',
        arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
        parameters=[{'log_level': LaunchConfiguration('log_level')}]
    )

    trajectory_controller_spawner = Node(
        package='controller_manager',
        executable='spawner',
        output='screen',
        arguments=[controller_name, '--controller-manager', '/controller_manager'],
        parameters=[{'log_level': LaunchConfiguration('log_level')}]
    )

    return LaunchDescription([
        DeclareLaunchArgument('description_file', default_value='controller.urdf.xacro'), 
        DeclareLaunchArgument('controllers_package', default_value='tilt_hw'), 
        DeclareLaunchArgument('controllers_file', default_value='robot_controller.yaml'),
        DeclareLaunchArgument('controller_name', default_value='forward_position_controller'),
        DeclareLaunchArgument('use_sim_time', default_value='false'),
        DeclareLaunchArgument('log_level', default_value='info'),

        rsp,
        control_node,
        joint_state_broadcaster_spawner,
        trajectory_controller_spawner,
        frame_provider,
    ])
