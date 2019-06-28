#!/bin/sh
terminator -e  " roslaunch control_pkg kvaser_can_bridge.launch " &
sleep 2
terminator -e  " roslaunch local_planning can_rxtx.launch " &
sleep 2
terminator -e  " rosrun local_planning local_planner " &
sleep 2
terminator -e  " rosrun pathplan pathplan "
