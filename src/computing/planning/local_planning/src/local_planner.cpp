#include <math.h>
#include <ros/ros.h>
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/DetectedObjectArray.h"
#include "kusv_msgs/kusv_CanInfo.h"
#include "local_planning/kusv_ControlCmd.h"

#define PI 3.14159265359

//#include "local_planning/kusv_Control_CanInfo.h"
// input : final_driving_way, objects, vehicle_states
// output : control_cmd


class LocalPlanner {
private:
    ros::NodeHandle nh;
    ros::Subscriber driving_way_sub;
    ros::Subscriber object_sub;
    ros::Subscriber can_rx_sub;
    ros::Publisher control_pub;

    kusv_msgs::PolyfitLaneData driving_way;
    kusv_msgs::DetectedObjectArray detected_objects;
    local_planning::kusv_ControlCmd control_cmd;
//    kusv_msgs::kusv_CanInfo canrx;


    double a0 = 0;
    double a1 = 0;
    double a2 = 0;
    double a3 = 0;
    double Vspeed = 0;
    double lookAhead_param = 0.25;
    double target_Speed = 5;

    double l_xd = 5;
    double l_d = 0;
    double el_d = 0;
    double g_x = 0;
    double g_y = 0;
    double alpha = 0;
    double lookAhead = 5;
    double wheelbase = 3;
    double steering = 0;

//    void canrxCallback(const kusv_msgs::kusv_CanInfo &rx);
//    void drivingWayCallback(const kusv_msgs::PolyfitLaneData::ConstPtr& lane);
//    void objectCallback(const kusv_msgs::DetectedObjectArray::ConstPtr& objects);
//    void collisionAvoid();
//    void purePursuit();
//    void pubControlCmd();

public:

    LocalPlanner()
        //  :driving_way_sub(nh.subscribe("/final_driving_way", 1000, &LocalPlanner::drivingWayCallback, this)),
         :driving_way_sub(nh.subscribe("/waypoint_lane", 1000, &LocalPlanner::drivingWayCallback, this)),
       //:driving_way_sub(nh.subscribe("vision_lane", 1000, &LocalPlanner::drivingWayCallback, this)),
         object_sub(nh.subscribe("/objects", 1000, &LocalPlanner::objectCallback, this)),
         can_rx_sub(nh.subscribe("/kusv_CanInfo", 1000, &LocalPlanner::canrxCallback, this)),
         control_pub(nh.advertise<local_planning::kusv_ControlCmd>("/control_cmd", 1000))
    {
    }


    void canrxCallback(const kusv_msgs::kusv_CanInfo rx){
//        canrx=*rx;
//        Vspeed =(canrx.speedrr+canrx.speedrl)/2;
        Vspeed = (rx.speedrr + rx.speedrl)/2;
    }


    void drivingWayCallback(const kusv_msgs::PolyfitLaneData::ConstPtr& lane) {
        driving_way=*lane;
        a3 = driving_way.a;
        a2 = driving_way.b;
        a1 = driving_way.c;
        a0 = driving_way.d;
    }


    void objectCallback(const kusv_msgs::DetectedObjectArray::ConstPtr& objects) {
        detected_objects=*objects;
        //transformVelodyneObjectsPoints
    }


    void collisionAvoid() {
    }


    void purePursuit() {
//        double lookahead_x = 0.3*(Vspeed + 0.0001);//0.3 is the parameter value
//        if(lookahead_x < lookAhead_param){
//            lookahead_x = lookAhead_param;
//        }
//        double g_x = lookahead_x; //x of goal point
//        double g_y = a0 + g_x*a1 + g_x*g_x*a2 + g_x*g_x*g_x*a3; //y of goal point
//        double lookahead_distance = pow(g_x*g_x + g_y*g_y, 0.5);
//        double lookahead_error = g_y+0.0001;
//        double alpha = atan2(lookahead_error, lookahead_distance);

//        steering = atan2(2 * wheelbase * sin(alpha), lookahead_distance);
        l_xd = 5;
        g_x = l_xd;
        g_y = a3 * l_xd * l_xd * l_xd + a2 * l_xd * l_xd + a1 * l_xd + a0;
        l_d = sqrt(g_x * g_x + g_y * g_y);
        el_d = g_y;
        alpha = asin(el_d / l_d);

        // steering = atan(2 * wheelbase * sin(alpha) / l_d);
        // steering = atan(2 * wheelbase * (a0 * l_d * l_d * l_d + a1 * l_d * l_d + a2 * l_d + a3)/(a0 * l_d * l_d * l_d + (a1 + 1)* l_d * l_d + a2 * l_d + a3));
        steering = atan(2 * wheelbase * g_y / (g_x * g_x + g_y * g_y));
        control_cmd.kusv_angular_z = steering * 12 * 180 / PI;

        if(((steering * 12 * 180 / PI) > 15) ||((steering * 12 * 180 / PI) < -15)){
            target_Speed -= 0.05;
            if(target_Speed <= 5)
                target_Speed = 5;
        }
        else{
            target_Speed += 0.02;
                if(target_Speed >= 10)
                    target_Speed = 10;
        }
        control_cmd.kusv_linear_x = target_Speed;
    }


    void pubControlCmd() {
        control_pub.publish(control_cmd);
    }
};

int main(int argc, char** argv){
    ros::init(argc, argv, "LocalPlanner");
    LocalPlanner lp;
    ros::Rate loop_rate(100);

    while(ros::ok())
    {
        lp.pubControlCmd();
        lp.purePursuit();
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
