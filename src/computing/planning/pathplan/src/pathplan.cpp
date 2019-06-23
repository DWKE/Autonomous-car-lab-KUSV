#include <ros/ros.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include "pathplan/kusv_ObjectCmd.h"
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/DetectedObjectArray.h"
#include <vector>

#define BASE_FRAME "velodyne"
#define LOCAL_FRAME "car"

// input : final_driving_way, objects, vehicle_states
// output : control_cmd

class PathPlan {
private:
    ros::NodeHandle nh;
    ros::Subscriber object_sub;
    ros::Subscriber lane_sub;                                                       //js
    ros::Publisher marker_pub; //visual array
    ros::Publisher visual_pub; //visual point
    ros::Publisher object_pub;                                                      //js

    kusv_msgs::DetectedObjectArray detected_objects;
    pathplan::kusv_ObjectCmd object_cmd;

    tf::TransformListener *m_pListener;

    //planning point
    std::vector<std::vector<geometry_msgs::Point>> objectsPoints;
    std::vector<std::vector<geometry_msgs::Point>> filteredPoints;
    geometry_msgs::Point checkedPoint;
    visualization_msgs::MarkerArray m_shapes;

    bool isObstacle = false;
    double distance = 0;
    double a0 = 0;                                                                  //js
    double a1 = 0;                                                                  //js
    double a2 = 0;                                                                  //js
    double a3 = 0;                                                                  //js

public:
    PathPlan()
        :object_sub(nh.subscribe("/DetectedObject", 1000, &PathPlan::objectCallback, this)),
         //lane_sub(nh.subscribe("/final_driving_way", 1000, &PathPlan::laneCallback, this)),
         //lane_sub(nh.subscribe("vision_lane", 1000, &PathPlan::laneCallback, this)),
         lane_sub(nh.subscribe("waypoint_lane", 1000, &PathPlan::laneCallback, this)),                 //js
         visual_pub(nh.advertise<visualization_msgs::Marker>("/check_point", 1)),
         marker_pub(nh.advertise<visualization_msgs::MarkerArray>("/mark_array", 1)),
         object_pub(nh.advertise<pathplan::kusv_ObjectCmd>("/object_cmd", 500))                             //js
    {

        m_pListener = new tf::TransformListener();

        tf::StampedTransform transform;
        m_pListener->waitForTransform (BASE_FRAME, LOCAL_FRAME, ros::Time(0), ros::Duration (10.0));
        m_pListener->lookupTransform (BASE_FRAME, LOCAL_FRAME, ros::Time(0), transform);
    }

    void objectCallback(const kusv_msgs::DetectedObjectArray::ConstPtr& input) {

        input2base(input);
        pointFilter();
        pointArrayVisualize(filteredPoints);
        selectPoint();
        pointVisualize(checkedPoint);                                                                                    //js
    }

    void laneCallback(const kusv_msgs::PolyfitLaneData::ConstPtr& lane){                                    //js
        a0 = lane->d;
        a1 = lane->c;
        a2 = lane->b;
        a3 = lane->a;
    }

    bool checkInline(double x, double y){

        bool inLine = false;
//        double d_line = 0;
        double d_line = a0 + a1*x + a2*pow(x,2) + a3*pow(x,3);                                              //js
        //double detect = sqrt(1+d_line);

        if(fabs(d_line-y)<=1){
            inLine = true;
            //ROS_INFO("true");
        } else{
            inLine = false;
        }

        return inLine;
    }

    void pointFilter(){

        filteredPoints.clear();

        for (const auto& objectPoints : objectsPoints)
        {
                std::vector<geometry_msgs::Point> filteredObjectPoints;

                for (const auto& point : objectPoints)
                {
                        geometry_msgs::Point yetpoint;
                        yetpoint.x = point.x;
                        yetpoint.y = point.y;
                        yetpoint.z = point.z;

                        //std::cout<<"lidar"<<std::endl;

                        if(yetpoint.z<0 && yetpoint.x>2){



                            if(checkInline(yetpoint.x, yetpoint.y)){
                               std::cout<<"A ";
                               filteredObjectPoints.push_back(yetpoint);
                            }
                        }
                }

                if(filteredObjectPoints.size()>0){
                    std::cout<<"B "<<std::endl;
                    filteredPoints.push_back(filteredObjectPoints);
                    isObstacle = true; //js
                    //std::cout << "test" << std::endl;
                    //std::cout<<"obstacle in line"<<std::endl;
                    //publish contorolcmd
                    //control_cmd.isObstacle=true;
                }/*
                else {
                    isObstacle = false;                                                                    //js
                }*/
        }

        if(filteredPoints.size()==0){
            isObstacle = false;
            //std::cout<<"any obstacle"<<std::endl;
        }

    }

    void selectPoint(){
        //input : filteredPoints
        //output : geometry_msgs::Point checkedPoint

        double lowXpoint = 20;

        for (const auto& objectPoints : filteredPoints)
        {
                std::vector<geometry_msgs::Point> filteredObjectPoints;
                geometry_msgs::Point yetpoint;

                for (const auto& point : objectPoints)
                {
                        yetpoint.x = point.x;

                        if(lowXpoint>=yetpoint.x){
                            lowXpoint=yetpoint.x;
                            checkedPoint = yetpoint;
                            std::cout<<"checkedPoint Xval : "<<yetpoint.x;
                            calcDistan(checkedPoint);
                        }
                }
        }
    }

    void calcDistan(const geometry_msgs::Point point){

        double distan = 0;
        double d_line = a1 + 2*a2*point.x + 3*a3*pow(point.x,2);

        if(a1==0){
            distan = sqrt(pow(point.x,2)+pow(point.y,2));
        } else {
            distan = pow(1+pow(d_line,2),0.5);
        }

        //distan = pow(1+pow(d_line,2),0.5);
        distance = distan;                                                                  //js

        std::cout<<", distance : "<<distan<<std::endl;

        //publish contorolcmd
        //control_cmd.distance=distan;
    }

    void input2base(const kusv_msgs::DetectedObjectArray::ConstPtr& input){
        objectsPoints.clear();

        // transformation point
        geometry_msgs::PointStamped laser_point;
        geometry_msgs::PointStamped base_point;
        laser_point.header.frame_id = BASE_FRAME;
        laser_point.header.stamp = ros::Time(0);

        // transformation pose
        geometry_msgs::PoseStamped laser_pose;
//        geometry_msgs::PoseStamped base_pose;
        laser_pose.header.frame_id = BASE_FRAME;
        laser_pose.header.stamp = ros::Time(0);


        for (const auto& object : input->objects)
        {
            std::vector<geometry_msgs::Point> objectPoints;

                // transform point and push
                for (const auto& point : object.convex_hull)
                {
                        laser_point.point.x = point.x;
                        laser_point.point.y = point.y;
                        laser_point.point.z = point.z;

                        m_pListener->transformPoint(LOCAL_FRAME, laser_point, base_point);
                        objectPoints.push_back (base_point.point);

                }

                objectsPoints.push_back(objectPoints);
        }

        //ROS_INFO("success input2base");

        // transform pose and push
//        laser_pose.pose = object.pose;

//        m_pListener->transformPose(LOCAL_FRAME, laser_pose, base_pose);
//        objectPoints.push_back (base_pose.pose);
    }

    void pointVisualize(const geometry_msgs::Point point){

        visualization_msgs::Marker checkpoint;
        checkpoint.header.stamp = ros::Time::now();
        checkpoint.header.frame_id= LOCAL_FRAME;
        checkpoint.ns = "/checked_point";
        checkpoint.id = 0;

        checkpoint.pose.position.x = point.x;
        checkpoint.pose.position.y = point.y;
        checkpoint.pose.position.z = point.z;

        checkpoint.type = visualization_msgs::Marker::SPHERE;
        checkpoint.action = visualization_msgs::Marker::ADD;
        checkpoint.scale.x = 1.15;
        checkpoint.scale.y = 1.15;
        checkpoint.scale.z = 1.15;

        checkpoint.color.a = 1.0;
        checkpoint.color.r = 0.0;
        checkpoint.color.g = 1.0;
        checkpoint.color.b = 0.0;

        checkpoint.lifetime = ros::Duration(10);

        if(isObstacle) {visual_pub.publish(checkpoint);}

    }

    void point_pub(){}

    void pointArrayVisualize (const std::vector<std::vector<geometry_msgs::Point>> objectsPoints)
    {       m_shapes.markers.clear();

            int m_iObjectNum = 0;

            for (const auto& objectPoints : objectsPoints)
            {
                    visualization_msgs::Marker shape;

                    shape.lifetime = ros::Duration(0.05);
                    shape.header.frame_id = LOCAL_FRAME;
                    shape.ns = LOCAL_FRAME;
                    shape.id = m_iObjectNum;

                    shape.type = visualization_msgs::Marker::LINE_STRIP;
                    shape.action = visualization_msgs::Marker::ADD;
                    shape.scale.x = 0.08;

                    shape.color.r = 1.0f;
                    shape.color.g = 0.0f;
                    shape.color.b = 0.0f;
                    shape.color.a = 0.5;

                    for (const auto &point : objectPoints)
                    {
                            geometry_msgs::Point tmpPoint;
                            tmpPoint.x = point.x;
                            tmpPoint.y = point.y;
                            tmpPoint.z = point.z;

                            shape.points.push_back(tmpPoint);
                            //std::cout << "test" << std::endl;
                    }
                    m_shapes.markers.push_back(shape);

                    m_iObjectNum++;
            }

            marker_pub.publish(m_shapes);
    }

    void objectCmdPub(){                                                                            //js
        object_cmd.distance = distance;
        object_cmd.isObstacle = isObstacle;
        object_pub.publish(object_cmd);
    }

};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "pathplan");
    PathPlan lp;
    ros::Rate loop_rate(50);
    ROS_INFO_STREAM("running");                                                                     //js
    while(ros::ok())
    {
        ros::spinOnce();
        lp.objectCmdPub();
        loop_rate.sleep();
    }
    return 0;
}
