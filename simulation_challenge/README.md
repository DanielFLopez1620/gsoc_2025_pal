# Simulation Challenge

Simulation challenge oriented to simulate a robot in GZ Harmonic and add a custom sensor.

## Set up

Do not forget to check the installation process on the base [README.md](/README.md)

## Robot simulation

The steps required for a simulation are:

1. Create a URDF/SDF file that contains the robot description, including all the links and joints, materials, controllers and configurations.

2. Load the robot description.

3. Initialize the GZ Simulator

4. Spawn the robot by using the robot description.

5. Load the ros_gz bridge.

This steps are followed in the file [robot_demo.launch.py](/simulation_challenge/launch/robot_demo.launch.py) by considering a modification of the ORION robot, which is present at [orion_mod.urdf.xacro](/simulation_challenge/urdf/orion_mod.urdf.xacro).

After you have build the packages as specified in the installation, you can run:

~~~bash
source ~/ros2_ws/install/setup.bash
ros2 launch simulation_challenge robot_demo.launch.py
~~~

Explore with the robot by using the teleoperation panel and check the sensors on the RVIZ Window.

![orion_demo](/simulation_challenge/img/demo_robot.gif)

## Including sensor

To use the dummy_sensor, the plugin and sensor definition were added to the [orion_mod.urdf.xacro](/simulation_challenge/urdf/orion_mod.urdf.xacro). However, to use it you need to provide the proper condition. So let's do it:

1. Make sure to export the path plugin to GZ:

    ~~~bash
    cd /path/to/gsoc_2025_pal/sensor_challenge/DummySensorsystem
    export GZ_SIM_SYSTEM_PLUGIN_PATH=`pwd`/build
    ~~~

2. Execute the launch with the flag **dummy_sensor**, do not forget to build
and source the packages as presented in the installation.

    ~~~bash
    source ~/ros2_ws/install/setup.bash
    ros2 launch simulation_challenge robot_demo.launch.py dummy_sensor:=true
    ~~~

3. Open a new terminal and subscribe to the topic on ROS 2 or GZ:

    ~~~bash
    gz topic -l
    ~~~


**NOTE:** It wasn't possible to load the custom sensor in the URDF, it only loaded the System Plugin but ommitted the configuration.
