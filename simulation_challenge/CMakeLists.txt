cmake_minimum_required(VERSION 3.8)
project(simulation_challenge)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(g_mov_description REQUIRED)
find_package(orion_description REQUIRED)
find_package(orion_gz REQUIRED)

# Add directories
install(
  DIRECTORY launch rviz urdf worlds config
  DESTINATION share/${PROJECT\_NAME}
)

# Tests
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
