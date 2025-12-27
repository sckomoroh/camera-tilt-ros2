FROM osrf/ros:jazzy-desktop

SHELL ["/bin/bash", "-lc"]

ARG USERNAME=USERNAME
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git \
    python3-pip python3-venv \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-jazzy-ament-cmake \
    ros-jazzy-ament-package \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-jazzy-ros2-control \
    ros-jazzy-ros2-controllers \
    ros-jazzy-hardware-interface \
    ros-jazzy-controller-manager \
    && rm -rf /var/lib/apt/lists/*

RUN echo "ubuntu ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/ubuntu \
    && chmod 0440 /etc/sudoers.d/ubuntu
# RUN echo "=== USERS ===" && cat /etc/passwd

USER ubuntu
WORKDIR /ws    

RUN rosdep init || true
RUN rosdep update

RUN echo "source /opt/ros/jazzy/setup.bash" >> /home/ubuntu/.bashrc
