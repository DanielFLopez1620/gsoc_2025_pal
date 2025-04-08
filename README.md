# gsoc_2025_pal

A repository oriented to the development of the challenges for the postulation of the GSoC 2025 with Pal Robotics.

This is a simulation demo showcasing [ORION Robot](https://github.com/Tesis-ORION/orion_common/tree/dev), which is oriented for HRI task, it is currently on development in the undergradrauated thesis project of the author of this repository.

![orion_project](/simulation_challenge/img/orion_project.png)

## Installation

Please follow the next steps in order to excute the two challenges proposed at the RFID with Gazebo Harmonic:

1. Create your workspace:

    ~~~bash
    mkdir -p ~/ros2_ws/src
    cd ~/ros2_ws
    colcon build
    ~~~

2. Install the repository of ORION common in the source directory

    ~~~bash
    cd ~/ros2_ws/src

    # For now it is on branch dev
    git clone -b dev https://github.com/Tesis-ORION/orion_common.git
    ~~~

3. Install the repository of ORION simulation in the source directory

    ~~~bash
    cd ~/ros2_ws/src
    # For now, the project is in the development branch
    git clone -b https://github.com/Tesis-ORION/orion_gz.git
    ~~~

4. Install external packages dependencies in the source

    ~~~bash
    cd ~/ros2_ws/src
    # For now also on dev branch
    git clone -b dev https://github.com/DanielFLopez1620/G-Mov_Project.git
    ~~~

5. Install the challenge repository:

    ~~~bash
    cd ~/ros2_ws/src
    # For now also on dev branch
    git clone https://github.com/DanielFLopez1620/gsoc_2025_pal.git
    ~~~

6. Install all the dependencies:

    ~~~bash
    sudo apt update
    sudo apt install python3-rosdep -y
    cd ~/ros2_ws
    sudo rosdep init
    rosdep update
    rosdep install --from-paths src --ignore-src -r -y
    ~~~

7. After the installation is complete, build the package with the provided options to avoid errors with other packages in development:

    ~~~bash
    cd ~/ros2_ws
    colcon build --symlink-install --packages-select g_mov_description orion orion_description orion_control orion_gz simulation_challenge
    source install/setup.bash
    ~~~

8. You are ready to explore the usage of the robot.
