# ///////////////////////////// REQUIRED LIBRARIES //////////////////////////////
# .............................. Python libraries ...............................
import os
import xacro

# ............................ Launch dependencies .............................
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource

# ........................... Additional packages dependencies ..................
from nav2_common.launch import ReplaceString 

# //////////////////////////// GLOBAL DEFINITIONS //////////////////////////////
ARGS = [
    DeclareLaunchArgument('dummy_sensor', default_value='false',
        description="Whether to use or not the dummy sensor of the challenge"),
    DeclareLaunchArgument('world', default_value='simple_world.sdf',
        description='Specify the world file for Gazebo'),
    DeclareLaunchArgument('x', default_value='0.0', 
        description='Initial X position'),
    DeclareLaunchArgument('y', default_value='0.0',
        description='Initial Y position'),
    DeclareLaunchArgument('z', default_value='0.5',
        description='Initial Z position'),
    DeclareLaunchArgument('R', default_value='0.0',
        description='Initial Roll'),
    DeclareLaunchArgument('P', default_value='0.0',
        description='Initial Pitch'),
    DeclareLaunchArgument('Y', default_value='3.1416',
        description='Initial Yaw'),
    DeclareLaunchArgument('entity', default_value='orion',
        description='Entity name or your preferred name for the robot'),
]

# /////////////////////////// FUNCTIONS DEFINITIONS ////////////////////////////
def get_argument(context, arg):
    """
    Get the context when performing the Launch Configuration
    """
    return LaunchConfiguration(arg).perform(context)

def generate_robot_description(context):
    """
    For generating the robot description, consider the URDF/Xacro file provided,
    but modifying the meshes source path in order to make it available to
    Gazebo Harmonic.
    """
    # Paths to consider
    pkg_gmov = get_package_share_directory('g_mov_description')
    pkg_description = get_package_share_directory('orion_description')
    pkg_sim_description = get_package_share_directory('simulation_challenge')
    xacro_file = os.path.join(pkg_sim_description, 'urdf', 'orion_mod.urdf.xacro')

    # Generating mapping in order to allow xacro modularity
    mappings = {
        'dummy_sensor': get_argument(context, "dummy_sensor")
    }

    # Obtaining robot description and making the substitution
    robot_description_config = xacro.process_file(xacro_file, mappings=mappings)
    robot_desc = robot_description_config.toprettyxml(indent='  ')
    robot_desc = robot_desc.replace(
        'package://orion_description/', f'file://{pkg_description}/'
    )
    robot_desc = robot_desc.replace(
        'package://g_mov_description/', f'file://{pkg_gmov}/'
    )

    # Launch node for robot state publisher
    rsp_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name="robot_state_publisher",
        output='screen',
        parameters=[{
            'robot_description': robot_desc,
            'rate': 50,
        }]
    )

    # Return configuration as a set
    return [rsp_node]

# /////////////////////////// FUNCTION DEFINITIONS ////////////////////////////
def replace_entities(path):
    """
    Oriented to make the proper substitutions on the bridge config file when
    using ros_gz bridge.
    """
    config_file = ReplaceString(
        source_file=path,
        replacements={
           '<entity>': LaunchConfiguration('entity'),
           '<world>': LaunchConfiguration('world')}
        )
    return ReplaceString(
        source_file=config_file,
        replacements={
           '.sdf': ''}
        )

# /////////////////////////// LAUNCH DEFINITIONS //////////////////////////////
def generate_launch_description():
    """
    Launch description for using ORION for the PAL Robotics GSOC 2025
    challenge on the RFID Harmonic section.
    """
    # Paths definitions
    challenge_pkg = get_package_share_directory("simulation_challenge")
    pkg_gz = get_package_share_directory('ros_gz_sim')
    gz_file = os.path.join(pkg_gz, 'launch', 'gz_sim.launch.py')
    world_path = PathJoinSubstitution([challenge_pkg,'worlds', LaunchConfiguration('world')])
    bridge_path = os.path.join(challenge_pkg, 'config', 'ros_gz_bridge.yaml')

    # Addiitional configurations
    bridge_config = replace_entities(bridge_path)

    # Generate launch description
    ld = LaunchDescription(ARGS)

    # Add robot description with context
    ld.add_action(OpaqueFunction(function=generate_robot_description))
    
    # Include GZ Launch
    ld.add_action(
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(gz_file),
            launch_arguments={
                'gz_args': ['-r -v 4 ', world_path],
                'on_exit_shutdown': 'true'
            }.items()
        )
    )

    ld.add_action(
       Node(
               package='ros_gz_bridge',
               name="ros_gz_bridge_for_clock",
               executable='parameter_bridge',
               arguments=['/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock'],
               output='screen',
           ),
   )

    # Launch ORION spawner
    ld.add_action(
        Node(
            package='ros_gz_sim',
            executable='create',
            arguments=[
                '-name', LaunchConfiguration('entity'),
                '-x', LaunchConfiguration('x'),
                '-y', LaunchConfiguration('y'),
                '-z', LaunchConfiguration('z'),
                '-R', LaunchConfiguration('R'),
                '-P', LaunchConfiguration('P'),
                '-Y', LaunchConfiguration('Y'),
                '-topic', 'robot_description',
                '-allow_renaming', 'false',
            ],
            output='screen',
        )
    )

    ld.add_action(
       Node(
               package='ros_gz_bridge',
               name="ros_gz_bridge_general",
               executable='parameter_bridge',
               parameters=[{
                'config_file': bridge_config
                }],
                output='screen',
           ),
    )

    return ld