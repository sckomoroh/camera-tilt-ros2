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
    
RUN pip install ultralytics --no-cache-dir

RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

RUN apt-get update && sudo apt install -y \
    python3-gi \
    python3-gst-1.0 \
    gir1.2-gstreamer-1.0 \
    gir1.2-gst-rtsp-server-1.0 \
    ros-jazzy-xacro \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    && rm -rf /var/lib/apt/lists/*


RUN echo "ubuntu ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/ubuntu \
    && chmod 0440 /etc/sudoers.d/ubuntu

USER ubuntu
WORKDIR /ws    

# RUN rosdep init || true
# RUN rosdep update

# RUN echo "source /opt/ros/jazzy/setup.bash" >> /home/ubuntu/.bashrc
