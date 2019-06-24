#include "simulator.hpp"
#include <ros/ros.h>

int main(int argc, char **argv)
{
	ros::init (argc, argv, "simulator");
	Simulator simulator;

	simulator.mainLoop ();

	return 0;
}
