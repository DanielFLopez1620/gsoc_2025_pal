# Dummy Sensor System Definition

Implementation to use a Custom Dummy sensor with GZ Sim.

The sensor used is defined in [DummySensor](/sensor_challenge/DummySensorSystem/DummySensor/README.md)

## Build

Follow the next steps to use the sensor system:

~~~bash
cd /path/to/gsoc_2025_pal/sensor_challenge/DummySensorsystem
mkdir build
cd build
cmake ..
make
~~~

## Usage

After you have *build*, you can use the demo world provided called [demo_dummy.sdf](/sensor_challenge/DummySensorSystem/demo_dummy.sdf). Follow the next steps:

1. Export the plugin to the *plugin path*:

    ~~~bash
    cd /path/to/gsoc_2025_pal/sensor_challenge/DummySensorsystem
    export GZ_SIM_SYSTEM_PLUGIN_PATH=`pwd`/build
    ~~~

2. Load and run the example world:

    ~~~bash
    gz sim -r demo_dummy.sdf
    ~~~

3. List to the custom message

    ~~~bash
    gz topic -l # Search for the dummy topic
    gz topic -e -t /world/demo_dummy_world/model/cube/link/link/sensor/my_dummy/dummy
    ~~~
