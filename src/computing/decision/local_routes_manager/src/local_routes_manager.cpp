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
    geometry_msgs::Pose change_cp[3];
    geometry_msgs::Pose tmp_change_cp[7];
    geometry_msgs::Pose slow_cp;
    float range_rad;
    int change_cnt=0;
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
        change_cnt=0;

        change_cp[0].position.x = 95.8791253799;
        change_cp[0].position.y = 169.909044062;
        change_cp[0].position.z = 0;

        change_cp[1].position.x = 99.9883675242;
        change_cp[1].position.y = 194.020044083;
        change_cp[1].position.z = 0;

        change_cp[2].position.x = 28.1230300859;
        change_cp[2].position.y = 37.6792495612;
        change_cp[2].position.z = 0;

//        tmp_change_cp[0].position.x = 82.2075794357;
//        tmp_change_cp[0].position.y = 149.500596819;
//        tmp_change_cp[0].position.z = 0;

//        tmp_change_cp[1].position.x = 50.0302641464;
//        tmp_change_cp[1].position.y = 70.7547282793;
//        tmp_change_cp[1].position.z = 0;

//        tmp_change_cp[2].position.x = 43.6937310826;
//        tmp_change_cp[2].position.y = 51.0765881946;
//        tmp_change_cp[2].position.z = 0;

//        tmp_change_cp[3].position.x = 74.810752737;
//        tmp_change_cp[3].position.y = 120.099917256;
//        tmp_change_cp[3].position.z = 0;

//        tmp_change_cp[4].position.x = 85.5657905416;
//        tmp_change_cp[4].position.y = 157.857981867;
//        tmp_change_cp[4].position.z = 0;

//        tmp_change_cp[5].position.x = 71.9384760213;
//        tmp_change_cp[5].position.y = 124.57272715;
//        tmp_change_cp[5].position.z = 0;

//        tmp_change_cp[6].position.x = 50.1716599016;
//        tmp_change_cp[6].position.y = 72.0088906598;
//        tmp_change_cp[6].position.z = 0;

        slow_cp.position.x = 104.707503191;
        slow_cp.position.y = 202.452903301;

        range_rad = 1.5;
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
        float distance = sqrt(pow(change_cp[change_cnt].position.x - curr_pose.pose.position.x,2) + pow(change_cp[change_cnt].position.y - curr_pose.pose.position.y,2));
        if(!mode_change_flag && distance<range_rad) {
            driving_mode = !driving_mode;
            mode_change_flag = true;
            ROS_INFO("mode changed to %d",driving_mode);
            change_cnt++;
        }
        else if(mode_change_flag && distance>range_rad) {
            mode_change_flag = false;
        }

//        float tmp_distance = sqrt(pow(tmp_change_cp[tmp_change_cnt].position.x - curr_pose.pose.position.x,2) + pow(tmp_change_cp[tmp_change_cnt].position.y - curr_pose.pose.position.y,2));
//        if(!tmp_mode_change_flag && tmp_distance<range_rad) {
//            driving_mode = !driving_mode;
//            tmp_mode_change_flag = true;
//            ROS_INFO("mode changed to %d",driving_mode);

//        }else if(tmp_mode_change_flag && tmp_distance>range_rad) {
//            driving_mode = !driving_mode;
//            tmp_mode_change_flag = false;
//            ROS_INFO("mode changed to %d",driving_mode);
//            tmp_change_cnt++;
//        }
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
