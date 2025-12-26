cmake_minimum_required(VERSION 3.8)
project(tilt_hw)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

message(STATUS "CMAKE_PREFIX_PATH=$ENV{CMAKE_PREFIX_PATH}")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(rclcpp REQUIRED)
find_package(pluginlib REQUIRED)
find_package(esp_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories(
  src
)

add_library(${PROJECT_NAME} SHARED
    src/interface/TiltHWInterface.cpp
    src/node/BridgeNode.cpp
)

ament_target_dependencies(${PROJECT_NAME}
    hardware_interface
    rclcpp
    pluginlib
    esp_msgs
)

pluginlib_export_plugin_description_file(hardware_interface tilt_hardware.xml)

install(
    TARGETS
    ${PROJECT_NAME}
    DESTINATION lib
)

install(
    DIRECTORY controllers
    DESTINATION share/${PROJECT_NAME}
)

install(
    DIRECTORY controllers
    DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY urdf controllers
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
