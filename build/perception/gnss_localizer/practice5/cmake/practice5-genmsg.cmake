# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "practice5: 2 messages, 0 services")

set(MSG_I_FLAGS "-Ipractice5:/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(practice5_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_custom_target(_practice5_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "practice5" "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" ""
)

get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_custom_target(_practice5_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "practice5" "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" "std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/practice5
)
_generate_msg_cpp(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/practice5
)

### Generating Services

### Generating Module File
_generate_module_cpp(practice5
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/practice5
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(practice5_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(practice5_generate_messages practice5_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_dependencies(practice5_generate_messages_cpp _practice5_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_dependencies(practice5_generate_messages_cpp _practice5_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(practice5_gencpp)
add_dependencies(practice5_gencpp practice5_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS practice5_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/practice5
)
_generate_msg_eus(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/practice5
)

### Generating Services

### Generating Module File
_generate_module_eus(practice5
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/practice5
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(practice5_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(practice5_generate_messages practice5_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_dependencies(practice5_generate_messages_eus _practice5_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_dependencies(practice5_generate_messages_eus _practice5_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(practice5_geneus)
add_dependencies(practice5_geneus practice5_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS practice5_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/practice5
)
_generate_msg_lisp(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/practice5
)

### Generating Services

### Generating Module File
_generate_module_lisp(practice5
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/practice5
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(practice5_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(practice5_generate_messages practice5_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_dependencies(practice5_generate_messages_lisp _practice5_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_dependencies(practice5_generate_messages_lisp _practice5_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(practice5_genlisp)
add_dependencies(practice5_genlisp practice5_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS practice5_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/practice5
)
_generate_msg_nodejs(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/practice5
)

### Generating Services

### Generating Module File
_generate_module_nodejs(practice5
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/practice5
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(practice5_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(practice5_generate_messages practice5_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_dependencies(practice5_generate_messages_nodejs _practice5_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_dependencies(practice5_generate_messages_nodejs _practice5_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(practice5_gennodejs)
add_dependencies(practice5_gennodejs practice5_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS practice5_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5
)
_generate_msg_py(practice5
  "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/kinetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5
)

### Generating Services

### Generating Module File
_generate_module_py(practice5
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(practice5_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(practice5_generate_messages practice5_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/StartDR.msg" NAME_WE)
add_dependencies(practice5_generate_messages_py _practice5_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/lke/ACL_KUSV/src/perception/gnss_localizer/practice5/msg/DR_data.msg" NAME_WE)
add_dependencies(practice5_generate_messages_py _practice5_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(practice5_genpy)
add_dependencies(practice5_genpy practice5_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS practice5_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/practice5)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/practice5
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(practice5_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(practice5_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/practice5)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/practice5
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(practice5_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(practice5_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/practice5)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/practice5
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(practice5_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(practice5_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/practice5)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/practice5
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(practice5_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(practice5_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/practice5
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(practice5_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(practice5_generate_messages_py std_msgs_generate_messages_py)
endif()
