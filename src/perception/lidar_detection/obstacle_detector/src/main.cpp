#include "tracker.hpp"
#include <ros/ros.h>

int main(int argc, char **argv)
{
	ros::init (argc, argv, "ObstacleDetector");
	Tracker tracker;

	tracker.mainLoop ();

	return 0;
}
