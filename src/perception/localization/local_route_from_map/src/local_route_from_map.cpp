#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/WaypointArray.h"

// input : global_waypoints, current_pose
// output : driving_way

class LocalRouteGenerator {
private:
    ros::NodeHandle nh;
    ros::Subscriber pose_sub;
    ros::Publisher driving_way_pub;

    geometry_msgs::PoseStamped curr_pose;
    kusv_msgs::PolyfitLaneData driving_way;

public:
    LocalRouteGenerator()
        :pose_sub(nh.subscribe("/current_pose", 1000, &LocalRouteGenerator::poseCallback, this))
    {

    }

    void poseCallback(geometry_msgs::PoseStampedConstPtr pose) {
        curr_pose=*pose;
    }

    void readCSV() {

    }

    void waypoints_to_lane() {

    }

    void pubDrivingWay() {
        driving_way_pub.publish(driving_way);
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "local_planner");

    ros::Rate loop_rate(100);

    LocalRouteGenerator rg;

    while(ros::ok())
    {
        rg.pubDrivingWay();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
