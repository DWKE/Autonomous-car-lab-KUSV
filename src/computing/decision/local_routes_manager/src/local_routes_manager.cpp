#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/Waypoint.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

#define LANE_KEEPING_MODE 0
#define WAYPOINTS_FOLLOWING_MODE 1

// input : camera_driving_way, pose, waypoints_driving_way
// output : final_driving_way
// todo : enu converting, range_cp tuning

class LocalRoutesManager {
private:
    ros::NodeHandle nh;
    ros::Subscriber lane_sub;
    ros::Subscriber pose_sub;
    ros::Subscriber wp_sub;
    ros::Publisher driving_way_pub;

    kusv_msgs::PolyfitLaneData final_driving_way;

    int driving_mode;
    bool mode_change_flag;

    geometry_msgs::PoseStamped curr_pose;
    geometry_msgs::Pose range_cp;
    float range_rad;

public:
    LocalRoutesManager()
        :lane_sub(nh.subscribe("/driving_way", 1000, &LocalRoutesManager::laneCallback, this)),
          pose_sub(nh.subscribe("/current_pose", 1000, &LocalRoutesManager::poseCallback, this)),
          wp_sub(nh.subscribe("/wp_driving_way", 1000, &LocalRoutesManager::wpCallback, this)),
          driving_way_pub(nh.advertise<kusv_msgs::PolyfitLaneData>("/final_driving_way", 1000))
    {
        driving_mode=LANE_KEEPING_MODE;
        mode_change_flag=false;

        //x: 14146112.5561
        //y: 4151255.99135
        range_cp.position.x = 14145987.7781;
        range_cp.position.y = 4151200.079;
        range_cp.position.z = 0;
        range_rad = 100;
    }

    void laneCallback(const kusv_msgs::PolyfitLaneData::ConstPtr& lane) {
        if(driving_mode==WAYPOINTS_FOLLOWING_MODE) return;
        final_driving_way=*lane;
    }

    void wpCallback(const kusv_msgs::PolyfitLaneData::ConstPtr& lane) {
        if(driving_mode==LANE_KEEPING_MODE) return;
        final_driving_way=*lane;
    }

    void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& pose) {
        curr_pose = *pose;
    }

    void modeChange() {
        float distance = sqrt(pow(range_cp.position.x - curr_pose.pose.position.x,2) + pow(range_cp.position.y - curr_pose.pose.position.y,2));
        if(!mode_change_flag && distance<range_rad) {
            driving_mode = !driving_mode;
            mode_change_flag = true;
            ROS_INFO("mode changed to %d",driving_mode);
        }else if(mode_change_flag && distance>range_rad) {
            mode_change_flag = false;
            ROS_INFO("mode change flag finished");
        }
    }

    void pubDrivingWay() {
        driving_way_pub.publish(final_driving_way);
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "LocalRoutesManager");
    ros::Time::init();

    ros::Rate loop_rate(100);

    LocalRoutesManager wm;

    while(ros::ok())
    {
        wm.modeChange();
        wm.pubDrivingWay();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
