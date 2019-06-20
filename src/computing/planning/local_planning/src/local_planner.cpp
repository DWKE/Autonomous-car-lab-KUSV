#include <ros/ros.h>
#include "kusv_msgs/ControlCmd.h"
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/DetectedObjectArray.h"

// input : final_driving_way, objects
// output : control_cmd

class LocalPlanner {
private:
    ros::NodeHandle nh;
    ros::Subscriber driving_way_sub;
    ros::Subscriber object_sub;
    ros::Publisher control_pub;

    kusv_msgs::PolyfitLaneData driving_way;
    kusv_msgs::DetectedObjectArray detected_objects;
    kusv_msgs::ControlCmd control_cmd;

public:
    LocalPlanner()
        :driving_way_sub(nh.subscribe("/final_driving_way", 1000, &LocalPlanner::drivingWayCallback, this)),
          object_sub(nh.subscribe("/objects", 1000, &LocalPlanner::objectCallback, this)),
          control_pub(nh.advertise<kusv_msgs::ControlCmd>("/control_cmd", 1000))
    {

    }

    void drivingWayCallback(kusv_msgs::PolyfitLaneDataConstPtr lane) {
        driving_way=*lane;
    }

    void objectCallback(kusv_msgs::DetectedObjectArray objects) {
        detected_objects=*objects;
    }

    void collisionAvoid() {

    }

    void purePursuit() {

    }

    void pubControlCmd() {
        control_pub.publish(control_cmd);
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "LocalPlanner");

    ros::Rate loop_rate(100);

    LocalPlanner lp;

    while(ros::ok())
    {
        lp.pubControlCmd();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
