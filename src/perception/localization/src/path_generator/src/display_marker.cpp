#include "kusv_msgs/PolyfitLaneDataArray.h"
#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"
#include <math.h>
#include <ros/ros.h>

#define PI 3.1415926579

class Display {
protected:
  ros::NodeHandle m_rosNodeHandler;

  ros::Publisher m_rosPubDrivingWay;
  ros::Subscriber m_rosSubDrivingWay;

public:
  Display() {
    m_rosPubDrivingWay =
        m_rosNodeHandler.advertise<visualization_msgs::MarkerArray>(
            "waypoint_lane_marker", 1000);
    m_rosSubDrivingWay = m_rosNodeHandler.subscribe(
        "waypoint_lane", 1000, &Display::drivingWayCallback, this);
  }

  ~Display() {}


protected:
  kusv_msgs::PolyfitLaneData m_drivingWay;

public:
  void
  drivingWayCallback(const kusv_msgs::PolyfitLaneData::ConstPtr &msg) {
    m_drivingWay = *msg;
  }

  void mark_drivingWay(double interval = 0.1, double ROILength = 7.0) {

    double a0 = m_drivingWay.d;
    double a1 = m_drivingWay.c;
    double a2 = m_drivingWay.b;
    double a3 = m_drivingWay.a;

    double x = 0.0;
    double y = a0;

    double distance_square = x * x + y * y;
    int id = 0;
    visualization_msgs::MarkerArray markerArray;
    while (distance_square < ROILength * ROILength) {

      y = a0 + a1 * x + a2 * x * x + a3 * x * x * x;
      distance_square = x * x + y * y;

      visualization_msgs::Marker marker;
      marker.header.frame_id = m_drivingWay.frame_id;
      marker.header.stamp = ros::Time::now();

      marker.ns = m_drivingWay.id;
      marker.id = id;

      marker.type = visualization_msgs::Marker::CYLINDER;
      marker.action = visualization_msgs::Marker::ADD;

      marker.pose.position.x = x;
      marker.pose.position.y = y;
      marker.pose.position.z = 0.0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.1;
      marker.pose.orientation.w = 1.0;
      marker.color.r = 1.0f;
      marker.color.g = 0.0f;
      marker.color.b = 0.0f;
      marker.color.a = 1.0;
      marker.scale.x = 0.2;
      marker.scale.y = 0.2;
      marker.scale.z = 0.2;
      marker.lifetime = ros::Duration(0.2);

      markerArray.markers.push_back(marker);
      x += interval;
      id++;
    }

    m_rosPubDrivingWay.publish(markerArray);
  }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "waypoint_display");
  // Vehicle vehicle;
  Display display;

  // The approximate control time is 100 Hz
  ros::Rate loop_rate(100);
  while (ros::ok()) {

    display.mark_drivingWay();

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
