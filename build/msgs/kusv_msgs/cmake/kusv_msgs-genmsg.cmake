# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "kusv_msgs: 9 messages, 0 services")

set(MSG_I_FLAGS "-Ikusv_msgs:/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/kinetic/share/sensor_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Ijsk_recognition_msgs:/opt/ros/kinetic/share/jsk_recognition_msgs/cmake/../msg;-Ipcl_msgs:/opt/ros/kinetic/share/pcl_msgs/cmake/../msg;-Ijsk_footstep_msgs:/opt/ros/kinetic/share/jsk_footstep_msgs/cmake/../msg;-Iactionlib_msgs:/opt/ros/kinetic/share/actionlib_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(kusv_msgs_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" ""
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" "geometry_msgs/Quaternion:geometry_msgs/PoseStamped:geometry_msgs/Pose:std_msgs/Header:geometry_msgs/Point"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" "geometry_msgs/Quaternion:geometry_msgs/Pose:geometry_msgs/Point"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" ""
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" "geometry_msgs/Quaternion:geometry_msgs/Pose:kusv_msgs/DetectedObject:geometry_msgs/Point"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" "kusv_msgs/PolyfitLaneData"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" "geometry_msgs/PoseStamped:std_msgs/Header:geometry_msgs/Quaternion:geometry_msgs/Point:geometry_msgs/Pose:kusv_msgs/Waypoint"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_custom_target(_kusv_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "kusv_msgs" "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" "geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg"
  "${MSG_I_FLAGS}"
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_cpp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
)

### Generating Services

### Generating Module File
_generate_module_cpp(kusv_msgs
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(kusv_msgs_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(kusv_msgs_generate_messages kusv_msgs_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_cpp _kusv_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(kusv_msgs_gencpp)
add_dependencies(kusv_msgs_gencpp kusv_msgs_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS kusv_msgs_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg"
  "${MSG_I_FLAGS}"
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)
_generate_msg_eus(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
)

### Generating Services

### Generating Module File
_generate_module_eus(kusv_msgs
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(kusv_msgs_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(kusv_msgs_generate_messages kusv_msgs_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_eus _kusv_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(kusv_msgs_geneus)
add_dependencies(kusv_msgs_geneus kusv_msgs_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS kusv_msgs_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg"
  "${MSG_I_FLAGS}"
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)
_generate_msg_lisp(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
)

### Generating Services

### Generating Module File
_generate_module_lisp(kusv_msgs
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(kusv_msgs_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(kusv_msgs_generate_messages kusv_msgs_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_lisp _kusv_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(kusv_msgs_genlisp)
add_dependencies(kusv_msgs_genlisp kusv_msgs_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS kusv_msgs_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg"
  "${MSG_I_FLAGS}"
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)
_generate_msg_nodejs(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
)

### Generating Services

### Generating Module File
_generate_module_nodejs(kusv_msgs
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(kusv_msgs_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(kusv_msgs_generate_messages kusv_msgs_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_nodejs _kusv_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(kusv_msgs_gennodejs)
add_dependencies(kusv_msgs_gennodejs kusv_msgs_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS kusv_msgs_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg"
  "${MSG_I_FLAGS}"
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Pose.msg;/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)
_generate_msg_py(kusv_msgs
  "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
)

### Generating Services

### Generating Module File
_generate_module_py(kusv_msgs
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(kusv_msgs_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(kusv_msgs_generate_messages kusv_msgs_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/ControlCmd.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/Waypoint.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObject.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/DetectedObjectArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/PolyfitLaneDataArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/WaypointArray.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/msgs/kusv_msgs/msg/LanePointData.msg" NAME_WE)
add_dependencies(kusv_msgs_generate_messages_py _kusv_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(kusv_msgs_genpy)
add_dependencies(kusv_msgs_genpy kusv_msgs_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS kusv_msgs_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/kusv_msgs
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(kusv_msgs_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(kusv_msgs_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(kusv_msgs_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET jsk_recognition_msgs_generate_messages_cpp)
  add_dependencies(kusv_msgs_generate_messages_cpp jsk_recognition_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/kusv_msgs
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(kusv_msgs_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(kusv_msgs_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(kusv_msgs_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET jsk_recognition_msgs_generate_messages_eus)
  add_dependencies(kusv_msgs_generate_messages_eus jsk_recognition_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/kusv_msgs
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(kusv_msgs_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(kusv_msgs_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(kusv_msgs_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET jsk_recognition_msgs_generate_messages_lisp)
  add_dependencies(kusv_msgs_generate_messages_lisp jsk_recognition_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/kusv_msgs
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(kusv_msgs_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(kusv_msgs_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(kusv_msgs_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET jsk_recognition_msgs_generate_messages_nodejs)
  add_dependencies(kusv_msgs_generate_messages_nodejs jsk_recognition_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/kusv_msgs
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(kusv_msgs_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(kusv_msgs_generate_messages_py sensor_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(kusv_msgs_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET jsk_recognition_msgs_generate_messages_py)
  add_dependencies(kusv_msgs_generate_messages_py jsk_recognition_msgs_generate_messages_py)
endif()
