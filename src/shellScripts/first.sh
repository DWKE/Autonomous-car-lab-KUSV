#!/bin/sh

terminator -e " roscore " &
sleep 2

# rosbag
terminator -e " cd ~/ros/rosbag " &
terminator -e " rosbag play lane_detection.bag " &

terminator -e " cd ~/ros/rosbag " &
terminator -e " rosbag play ros_CAN_RX.bag" &

# vision
terminator -e " roslaunch lane_detector vision.launch " &
sleep 1

# planning
terminator -e  " roslaunch ros_can kvaser_can_bridge.launch " &
sleep 1
terminator -e  " rosrun local_planning can_tx " &
sleep 1
terminator -e  " rosrun local_planning local_planner " &
sleep 1
terminator -e  " rosrun pathplan pathplan "
sleep 1


