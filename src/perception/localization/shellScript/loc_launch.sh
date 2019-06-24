#!/bin/sh
terminator -e  " roslaunch control_pkg kvaser_can_bridge.launch " &
sleep 3
terminator -e  " roslaunch ublox_gps ublox_device.launch " &
sleep 3
terminator -e  " roslaunch pose_estimation pose_estimation_ekf.launch " &
sleep 3
terminator -e  " roslaunch local_route_from_map local_route_from_map.launch " &
sleep 3
terminator -e  " roslaunch path_generator path_generator.launch " 
