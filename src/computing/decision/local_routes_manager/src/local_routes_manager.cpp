#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/Waypoint.h"
#include "kusv_msgs/DrivingMode.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_broadcaster.h"

#define LANE_KEEPING_MODE 0
#define WAYPOINTS_FOLLOWING_MODE 1

// input : camera_driving_way, pose, waypoints_driving_way
// output : final_driving_way
// todo : enu converting, change_cp tuning

class LocalRoutesManager {
private:
    ros::NodeHandle nh;
    ros::Subscriber lane_sub;
    ros::Subscriber pose_sub;
    ros::Subscriber wp_sub;
    ros::Publisher driving_way_pub;
    ros::Publisher driving_mode_pub;

    kusv_msgs::PolyfitLaneData final_driving_way;
    kusv_msgs::DrivingMode mode;

    bool driving_mode;
    bool mode_change_flag;
    bool tmp_mode_change_flag;

    geometry_msgs::PoseStamped curr_pose;
    geometry_msgs::Pose change_cp;
    geometry_msgs::Pose tmp_change_cp[4];
    geometry_msgs::Pose slow_cp;
    float range_rad;
    int tmp_change_cnt=0;

public:
    LocalRoutesManager()
        :lane_sub(nh.subscribe("/vision_lane", 1000, &LocalRoutesManager::laneCallback, this)),
          pose_sub(nh.subscribe("/localization/local_pose", 1000, &LocalRoutesManager::poseCallback, this)),
          wp_sub(nh.subscribe("/waypoint_lane", 1000, &LocalRoutesManager::wpCallback, this)),
          driving_way_pub(nh.advertise<kusv_msgs::PolyfitLaneData>("/final_driving_way", 1000)),
          driving_mode_pub(nh.advertise<kusv_msgs::DrivingMode>("/driving_mode", 1000))
    {
        driving_mode=LANE_KEEPING_MODE;
        mode_change_flag=false;
        tmp_mode_change_flag=false;


        change_cp.position.x = 92.0;
        change_cp.position.y = 166.5;
        change_cp.position.z = 0;

        tmp_change_cp[0].position.x = 46.1064110262;
        tmp_change_cp[0].position.y = 54.6836921825;
        tmp_change_cp[0].position.z = 0;

        tmp_change_cp[1].position.x = 74.3688746371;
        tmp_change_cp[1].position.y = 119.578274418;
        tmp_change_cp[1].position.z = 0;

        tmp_change_cp[2].position.x = 50.3219096485;
        tmp_change_cp[2].position.y = 70.5660489841;
        tmp_change_cp[2].position.z = 0;

        tmp_change_cp[3].position.x = 23.1107076412;
        tmp_change_cp[3].position.y = 34.2175396228;
        tmp_change_cp[3].position.z = 0;

        slow_cp.position.x = 104.707503191;
        slow_cp.position.y = 202.452903301;

        range_rad = 10;
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
        float distance = sqrt(pow(change_cp.position.x - curr_pose.pose.position.x,2) + pow(change_cp.position.y - curr_pose.pose.position.y,2));
        if(!mode_change_flag && distance<range_rad*2) {
            driving_mode = !driving_mode;
            mode_change_flag = true;
            ROS_INFO("mode changed to %d",driving_mode);
        }
        else if(mode_change_flag && distance>range_rad) {
            mode_change_flag = false;
        }

        float tmp_distance = sqrt(pow(tmp_change_cp[tmp_change_cnt].position.x - curr_pose.pose.position.x,2) + pow(tmp_change_cp[tmp_change_cnt].position.y - curr_pose.pose.position.y,2));
        if(!tmp_mode_change_flag && tmp_distance<range_rad) {
            driving_mode = !driving_mode;
            tmp_mode_change_flag = true;
            ROS_INFO("mode changed to %d",driving_mode);

        }else if(tmp_mode_change_flag && tmp_distance>range_rad) {
            driving_mode = !driving_mode;
            tmp_mode_change_flag = false;
            ROS_INFO("mode changed to %d",driving_mode);
            tmp_change_cnt++;
        }
    }

    void pubDrivingWay() {
        driving_way_pub.publish(final_driving_way);
    }
    void pubDrivingMode() {
        mode.mode=driving_mode;
        driving_mode_pub.publish(mode);
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
        wm.pubDrivingMode();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
