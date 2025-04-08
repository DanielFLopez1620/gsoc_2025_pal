cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)

find_package(gz-cmake3 REQUIRED)

project(DummySensorSystem)

gz_find_package(gz-plugin2 REQUIRED COMPONENTS register)
set(GZ_PLUGIN_VER ${gz-plugin2_VERSION_MAJOR})

gz_find_package(gz-sim8 REQUIRED)
set(GZ_SIM_VER ${gz-sim8_VERSION_MAJOR})

find_package(gz-sensors8 REQUIRED)
set(GZ_SENSORS_VER ${gz-sensors8_VERSION_MAJOR})

add_subdirectory(DummySensor)
set_property(TARGET DummySensor PROPERTY CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cc)
target_link_libraries(${PROJECT_NAME}
    PUBLIC gz-plugin${GZ_PLUGIN_VER}::gz-plugin${GZ_PLUGIN_VER}
    PUBLIC gz-sim${GZ_SIM_VER}::gz-sim${GZ_SIM_VER}
    PUBLIC gz-sensors${GZ_SENSORS_VER}::gz-sensors${GZ_SENSORS_VER}
    PUBLIC DummySensor
)

target_include_directories(${PROJECT_NAME}
    PUBLIC DummySensor)