#include <ros/ros.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "obstacle_detector");

    ros::Rate loop_rate(100);

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
