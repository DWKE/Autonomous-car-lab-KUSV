#include "KusvLane.hpp"
#include "autonomous_msg/LanePointData.h"
#include "autonomous_msg/LanePointDataArray.h"
//#include "autonomous_msg/PolyfitLaneDataArray.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "ublox_msgs/NavPVT.h"
#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"
#include "nav_msgs/Odometry.h"

#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <tf/LinearMath/Quaternion.h>

#include <eigen3/Eigen/Dense>
#include <math.h>
#include <ros/ros.h>
#include <string>

#include <cstdlib>

class LaneDetection {

  ////////////////////////////////////////////////////////////////////////////////
  // 0. Constructor & destructure

public:
  //LaneDetection():m_rosSubEstimatedPose(m_rosNodeHandler.subscribe("/localization/pose_estimation/estimated_state", 10, &LaneDetection::EstimatedPose_Callback, this)) {
  LaneDetection() {
    
    // publisher 
    m_rosPubCsvLanes =
        m_rosNodeHandler.advertise<autonomous_msg::LanePointDataArray>("csv_lanes", 10);

    m_rosPubCsvLocalLanes =
        m_rosNodeHandler.advertise<autonomous_msg::LanePointDataArray>("csv_local_map", 10);    

    m_rosPubROILanes =
        m_rosNodeHandler.advertise<autonomous_msg::LanePointDataArray>("ROI_lanes", 10);

    m_rosPubLocalPose = 
        m_rosNodeHandler.advertise<geometry_msgs::PoseStamped>("local_pose", 10);

    m_rosPubLocalPoseOdom =
        m_rosNodeHandler.advertise<nav_msgs::Odometry>("local_pose_odometry", 10);

    m_rosPubCsvMarkerArray =
        m_rosNodeHandler.advertise<visualization_msgs::MarkerArray>(
            "csv_lanes_marker", 10);
    
    // subscriber
    m_rosSubEstimatedPose =
        m_rosNodeHandler.subscribe("/localization/pose_estimation/estimated_state", 10, &LaneDetection::EstimatedPose_Callback, this);


    // m_rosPubPolyLanes =
    //     m_rosNodeHandler.advertise<autonomous_msg::PolyfitLaneDataArray>(
    //         "polyfit_lanes", 10);

    m_rosNodeHandler.param("local_route_from_map/ns", m_vehicle_namespace_param,
                           std::string(""));
    m_rosNodeHandler.param("local_route_from_map/path", m_path_param,
                           std::string(""));
    if (m_path_param == std::string("")) {
      ROS_ERROR_STREAM("Empty path!!");
    }
  }

  ~LaneDetection() {}

protected:
  ros::NodeHandle m_rosNodeHandler;
  ros::Publisher m_rosPubROILanes;
  ros::Publisher m_rosPubCsvLanes;
  ros::Publisher m_rosPubCsvLocalLanes;
  ros::Publisher m_rosPubLocalPose;
  ros::Publisher m_rosPubLocalPoseOdom;
  ros::Publisher m_rosPubPolyLanes;
  ros::Publisher m_rosPubCsvMarkerArray;

  ros::Subscriber m_rosSubEstimatedPose;

  std::string m_vehicle_namespace_param;

  double m_ROIFront_param = 20.0;
  double m_ROIRear_param = 10.0;
  double m_ROILeft_param = 3.0;
  double m_ROIRight_param = 3.0;

  // ENU conversion
  double m_pi = 3.14159265358979323846;

  double m_origin_lat_deg = 37.540358;
  double m_origin_lat_rad = m_origin_lat_deg * m_pi/180.0;

  double m_origin_lon_deg = 127.074880;
  double m_origin_lon_rad = m_origin_lon_deg * m_pi/180.0;

  double m_a = 6378137.0;       // semi-major axis [m]
  double m_b = 6356752.314245;  // semi-minor axis [m]

  tf::TransformListener m_rosTfListenr;
  tf::TransformBroadcaster m_rosTfBroadcaster;

  std::string m_path_param;
  autonomous_msg::LanePointDataArray m_csvLanes;
  autonomous_msg::LanePointDataArray m_csvLocalLanes;
  autonomous_msg::LanePointDataArray m_ROILanes;
  //autonomous_msg::PolyfitLaneDataArray m_polyLanes;

  autonomous_msg::LanePointData m_rightLane;
  //autonomous_msg::PolyfitLaneData m_rightPoly;

public:
  void loadLaneData() {
    SKusvLanes csvLaneImport;
    csvLaneImport.ImportKusvLaneCsvFile(m_path_param);
    m_csvLanes.frame_id = "/world";
    m_csvLanes.lane.clear();
    for (auto i_lane = 0; i_lane < csvLaneImport.m_vecKusvLanes.size(); i_lane++) 
    {
      autonomous_msg::LanePointData lane;
      lane.frame_id = "world";
      lane.id = std::to_string(csvLaneImport.m_vecKusvLanes[i_lane].m_nLaneID);
      for (auto i_point = 0; i_point < csvLaneImport.m_vecKusvLanes[i_lane].m_vecKusvLanePoint.size(); i_point++) 
      {
        geometry_msgs::Point point;
        point.x = csvLaneImport.m_vecKusvLanes[i_lane]
                      .m_vecKusvLanePoint[i_point]
                      .m_dPtX_m;
        point.y = csvLaneImport.m_vecKusvLanes[i_lane]
                      .m_vecKusvLanePoint[i_point]
                      .m_dPtY_m;
        lane.point.push_back(point);
      }
      m_csvLanes.lane.push_back(lane);
    }
  }
  
  void enuConversion() {
    m_csvLocalLanes.frame_id = m_csvLanes.frame_id;
    m_csvLocalLanes.lane.clear();
    
    double temp_lon;
    double temp_lat;

    for (auto i_lane = 0; i_lane < m_csvLanes.lane.size(); i_lane++) 
    {
      autonomous_msg::LanePointData local_lane;
      local_lane.frame_id = m_csvLanes.lane[i_lane].frame_id;
      local_lane.id       = m_csvLanes.lane[i_lane].id;
      for (auto i_point = 0; i_point < m_csvLanes.lane[i_lane].point.size(); i_point++) 
      {
        geometry_msgs::Point local_point;
        temp_lon = m_csvLanes.lane[i_lane].point[i_point].x * 1e-7 * m_pi / 180.0;
        temp_lat = m_csvLanes.lane[i_lane].point[i_point].y * 1e-7 * m_pi / 180.0;
        
        local_point.x = (temp_lon - m_origin_lon_rad)*(NormalRadius(m_a, m_b, temp_lat)*cos(temp_lat));
        local_point.y = (temp_lat - m_origin_lat_rad)*MeridionalRadius(m_a, m_b, temp_lat);
        //local_point.x = m_csvLanes.lane[i_lane].point[i_point].x * 1e-7;
        //local_point.y = m_csvLanes.lane[i_lane].point[i_point].y * 1e-7;
        local_lane.point.push_back(local_point);
        // ROS_INFO("east : %f, north : %f", local_point.x, local_point.y);
        ROS_INFO("longitude: %f, latitude: %f", m_csvLanes.lane[i_lane].point[i_point].x, m_csvLanes.lane[i_lane].point[i_point].y);
      }
      m_csvLocalLanes.lane.push_back(local_lane);
      ROS_INFO("ENU conversion end");
    }
    ROS_INFO("longitude : %f, latitude : %f", m_csvLanes.lane[0].point[0].x*1e-7, m_csvLanes.lane[0].point[0].y*1e-7);
  }

  void publishCsvLanesData() {
    m_rosPubCsvLanes.publish(m_csvLanes);
    m_rosPubCsvLocalLanes.publish(m_csvLocalLanes); 
  }
  
  double MeridionalRadius(double a, double b, double lat){
    return pow(a*b, 2) / sqrt( pow((pow( a*cos(lat), 2) + pow( b*sin(lat), 2 )), 3));
  }

  double NormalRadius(double a, double b, double lat){
    return (a*a) / sqrt(pow( a*cos(lat), 2 ) + pow( b*sin(lat), 2));
  }


  void EstimatedPose_Callback(const ublox_msgs::NavPVT::ConstPtr &msg) {
    double dLon_raw = 0.0;
    double dLat_raw = 0.0;
    double dHeading_raw = 0.0;

    double dLon_deg = 0.0;
    double dLat_deg = 0.0;
    double dLon_rad = 0.0;
    double dLat_rad = 0.0;
    double dHeading_deg = 0.0;
    double dHeading_rad = 0.0;

    double dEast_m = 0.0;
    double dNorth_m = 0.0;


    dLon_raw = msg->lon;
    dLat_raw = msg->lat;
    //gSpeed_raw = msg->gSpeed;
    dHeading_raw = msg->heading;

    dLon_deg = ((double)dLon_raw)/10000000.0;
    dLon_rad = dLon_deg*m_pi/180.0;

    dLat_deg = ((double)dLat_raw)/10000000.0;
    dLat_rad = dLat_deg*m_pi/180.0;

    //dHeading_deg = -((double)dHeading_raw)/100000.0 + 90.0;
    dHeading_deg = ((double)dHeading_raw)/100000.0;
    dHeading_rad = dHeading_deg*m_pi/180.0;

    if (dHeading_rad < 0){
        dHeading_rad += 2*m_pi;
    }
    if (dHeading_rad > 2*m_pi){
        dHeading_rad -= 2*m_pi;
    }

    // LLH to ENU conversion
    dEast_m  = (dLon_rad - m_origin_lon_rad)*(NormalRadius(m_a, m_b, dLat_rad)*cos(dLat_rad));
    dNorth_m = (dLat_rad - m_origin_lat_rad)*MeridionalRadius(m_a, m_b, dLat_rad);
    //ROS_INFO("East : %f, North : %f", dEast_m, dNorth_m);

    // publish poseStamped msg
    geometry_msgs::PoseStamped local_pose;
    nav_msgs::Odometry local_pose_odom;
    tf::Quaternion q;
    q.setRPY(0, 0, dHeading_rad);
    
    local_pose.header.stamp = ros::Time::now();
    local_pose.header.frame_id = "/world";
    local_pose.pose.position.x = dEast_m;
    local_pose.pose.position.y = dNorth_m;
    local_pose.pose.orientation.x = q.getX();
    local_pose.pose.orientation.y = q.getY();
    local_pose.pose.orientation.z = q.getZ();
    local_pose.pose.orientation.w = q.getW();

    local_pose_odom.header.frame_id = "world";
    local_pose_odom.header.stamp = ros::Time::now();
    local_pose_odom.pose.pose = local_pose.pose;

    m_rosPubLocalPose.publish(local_pose);
    m_rosPubLocalPoseOdom.publish(local_pose_odom);


    // broadcast tf
    tf::Transform transform;

    transform.setOrigin(tf::Vector3(dEast_m, dNorth_m, 0.0));
    transform.setRotation(q);

    // broadcasting the vehicle's body coordinate system
    // The parent coordinate is world, child coordinate is body.
    m_rosTfBroadcaster.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/world", "/body"));
  }

  void mark_csvLanes() {

    visualization_msgs::MarkerArray markerArray;

    int id = 0;

    for (auto i_lane = 0; i_lane < m_csvLocalLanes.lane.size(); i_lane++) {
      visualization_msgs::Marker marker;
      marker.header.frame_id = m_csvLocalLanes.frame_id;
      marker.header.stamp = ros::Time::now();

      marker.ns = m_csvLocalLanes.lane[i_lane].id;
      marker.id = id++;

      marker.type = visualization_msgs::Marker::LINE_STRIP;
      marker.action = visualization_msgs::Marker::ADD;

      marker.color.r = 0.9f;
      marker.color.g = 0.9f;
      marker.color.b = 0.9f;
      marker.color.a = 0.7;
      marker.scale.x = 0.2;
      marker.lifetime = ros::Duration();

      geometry_msgs::Point prevPoint;
      bool first = true;

      for (auto i_point = 0; i_point < m_csvLocalLanes.lane[i_lane].point.size();
           i_point++) {
        geometry_msgs::Point currPoint;
        currPoint.x = m_csvLocalLanes.lane[i_lane].point[i_point].x;
        currPoint.y = m_csvLocalLanes.lane[i_lane].point[i_point].y;
        currPoint.z = 0.0;

        if (first == true) {
          first = false;
        } else {
          double dx = currPoint.x - prevPoint.x;
          double dy = currPoint.y - prevPoint.y;
          if ((dx * dx + dy * dy) <= 4.0 * 4.0) {
            marker.points.push_back(prevPoint);
            marker.points.push_back(currPoint);
          } else {
            markerArray.markers.push_back(marker);
            marker.points.clear();
            marker.id = id++;
          }
        }
        prevPoint = currPoint;
      }
      markerArray.markers.push_back(marker);
    }
    m_rosPubCsvMarkerArray.publish(markerArray);
  }
};



int main(int argc, char **argv) {

  ros::init(argc, argv, "local_route_from_map");

  //LaneDetection laneDetection;
  //laneDetection.loadLaneData();
  LaneDetection readGlobalWaypoints;
  readGlobalWaypoints.loadLaneData(); // store 'm_csvLanes' lane data(lane.point?) 
  readGlobalWaypoints.enuConversion();

  double prev_csvLaneMarkTime = ros::Time::now().toSec();
  // The approximate control time is 100 Hz
  ros::Rate loop_rate(100);

  while (ros::ok()) {
    // laneDetection.convertLaneWorld2Body();
    //laneDetection.extractRegionOfInterest();
    //laneDetection.polyfitLane();
    if ((ros::Time::now().toSec() - prev_csvLaneMarkTime) > 1.0) {
      prev_csvLaneMarkTime = ros::Time::now().toSec();
      readGlobalWaypoints.publishCsvLanesData();
      readGlobalWaypoints.mark_csvLanes();
    }
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
