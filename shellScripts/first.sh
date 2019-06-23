#!/bin/sh

terminator -e  " roslaunch ros_can kvaser_can_bridge.launch " &
sleep 2
terminator -e  " rosrun local_planning can_tx " &
sleep 2
terminator -e  " rosrun local_planning local_planner " &
sleep 2
terminator -e  " rosrun pathplan pathplan "


