#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/Waypoint.h"

#define LANE_KEEPING_MODE 0
#define WAYPOINTS_FOLLOWING_MODE 1

// input : camera_abcd, pose, generated_waypoints
// output : final_abcd

class WaypointsManager {
private:
    ros::NodeHandle nh;
    ros::Subscriber lane_sub;
    ros::Subscriber pose_sub;
    ros::Subscriber wp_sub;
    ros::Publisher lane_pub;

    kusv_msgs::PolyfitLaneData managed_lane;

    int driving_mode;
    bool mode_change_flag;

public:
    WaypointsManager()
        :lane_sub(nh.subscribe("/vision_lane", 1000, &WaypointsManager::laneCallback, this)),
          pose_sub(nh.subscribe("/current_pose", 1000, &WaypointsManager::poseCallback, this)),
          wp_sub(nh.subscribe("/global_waypoints", 1000, &WaypointsManager::wpCallback, this)),
          lane_pub(nh.advertise<kusv_msgs::PolyfitLaneData>("/final_lane", 1000))
    {
        driving_mode=LANE_KEEPING_MODE;
        mode_change_flag=false;
    }

    void laneCallback(kusv_msgs::PolyfitLaneDataConstPtr lane) {
        if(driving_mode) return;

        managed_lane=*lane;
    }

    void poseCallback(geometry_msgs::PoseConstPtr pose) {
        if(!driving_mode) return;


    }

    void wpCallback(kusv_msgs::WaypointConstPtr wp) {

    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "node");

    ros::Rate loop_rate(100);

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
