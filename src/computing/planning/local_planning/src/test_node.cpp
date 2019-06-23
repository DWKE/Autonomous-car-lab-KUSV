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
    double k = 0;
    geometry_msgs::PoseStamped curr_pose;
    geometry_msgs::Pose range_cp;
    float range_rad;
    bool state = true;

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

        double l_xd = 5;
        
        if(state)
            k += 0.001;
        else
            k -= 0.001;
        final_driving_way.a = 0;
        final_driving_way.b = 0;
        final_driving_way.c = k;
        final_driving_way.d = 0;//-1.57235227926;
        double g_y = final_driving_way.a * l_xd * l_xd * l_xd + final_driving_way.b * l_xd * l_xd + final_driving_way.c * l_xd + final_driving_way.d;
        
        if (g_y > 3.7)
            state = false;
        if (g_y < -3.7)
            state = true;

        ROS_INFO_STREAM("g_y = " << g_y);
        driving_way_pub.publish(final_driving_way);
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "LocalRoutesManager");
    LocalRoutesManager wm;
    ros::Time::init();
    ros::Rate loop_rate(100);


    while(ros::ok())
    {
//        wm.modeChange();
        wm.pubDrivingWay();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
