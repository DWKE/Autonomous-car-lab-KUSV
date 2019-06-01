# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "can: 1 messages, 0 services")

set(MSG_I_FLAGS "-Ican:/home/lke/ACL_KUSV/src/sensing/can/msg;-Ican_msgs:/opt/ros/kinetic/share/can_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(can_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_custom_target(_can_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "can" "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" "std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(can
  "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/can
)

### Generating Services

### Generating Module File
_generate_module_cpp(can
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/can
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(can_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(can_generate_messages can_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(can_generate_messages_cpp _can_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(can_gencpp)
add_dependencies(can_gencpp can_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS can_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(can
  "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/can
)

### Generating Services

### Generating Module File
_generate_module_eus(can
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/can
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(can_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(can_generate_messages can_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(can_generate_messages_eus _can_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(can_geneus)
add_dependencies(can_geneus can_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS can_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(can
  "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/can
)

### Generating Services

### Generating Module File
_generate_module_lisp(can
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/can
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(can_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(can_generate_messages can_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(can_generate_messages_lisp _can_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(can_genlisp)
add_dependencies(can_genlisp can_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS can_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(can
  "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/can
)

### Generating Services

### Generating Module File
_generate_module_nodejs(can
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/can
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(can_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(can_generate_messages can_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(can_generate_messages_nodejs _can_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(can_gennodejs)
add_dependencies(can_gennodejs can_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS can_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(can
  "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/can
)

### Generating Services

### Generating Module File
_generate_module_py(can
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/can
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(can_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(can_generate_messages can_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/sensing/can/msg/kusv_CanInfo.msg" NAME_WE)
add_dependencies(can_generate_messages_py _can_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(can_genpy)
add_dependencies(can_genpy can_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS can_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/can)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/can
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET can_msgs_generate_messages_cpp)
  add_dependencies(can_generate_messages_cpp can_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(can_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(can_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/can)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/can
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET can_msgs_generate_messages_eus)
  add_dependencies(can_generate_messages_eus can_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(can_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(can_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/can)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/can
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET can_msgs_generate_messages_lisp)
  add_dependencies(can_generate_messages_lisp can_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(can_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(can_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/can)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/can
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET can_msgs_generate_messages_nodejs)
  add_dependencies(can_generate_messages_nodejs can_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(can_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(can_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/can)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/can\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/can
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET can_msgs_generate_messages_py)
  add_dependencies(can_generate_messages_py can_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(can_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(can_generate_messages_py std_msgs_generate_messages_py)
endif()
