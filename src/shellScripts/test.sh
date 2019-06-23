#!/bin/sh

terminator -e " roscore " &
sleep 2

# rosbag
terminator -e " cd ~/ros/rosbag " &
terminator -e " rosbag play lane_detection.bag " &
sleep 1

terminator -e " cd ~/ros/rosbag " &
terminator -e " rosbag play ros_CAN_RX.bag "

sleep 1
