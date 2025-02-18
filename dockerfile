FROM ros:humble

# Install dependencies
RUN apt-get update && apt-get install -y \
    ros-humble-pcl-ros \
    ros-humble-tf2 \
    ros-humble-tf2-ros \
    ros-humble-rviz2 \
    pcl-tools \
    libpcl-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /ros2_ws/src/multi_lidar_calibration
COPY . .

WORKDIR /ros2_ws
SHELL ["/bin/bash", "-c"]
RUN source /opt/ros/humble/setup.bash && \
    colcon build

RUN echo -e "source /opt/ros/humble/setup.bash" >> ~/.bashrc && \
    echo "source /ros2_ws/install/setup.bash" >> ~/.bashrc

# Setup entrypoint
# ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
