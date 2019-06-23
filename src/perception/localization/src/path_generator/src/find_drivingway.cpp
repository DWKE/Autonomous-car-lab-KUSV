#include <ros/ros.h>
#include "std_msgs/Float64.h"
#include <math.h>
#include <ros/ros.h>
#include <string>
#include "kusv_msgs/PolyfitLaneDataArray.h"
#include "kusv_msgs/PolyfitLaneData.h"

class find_drivingway {
protected:
  ros::NodeHandle m_rosNodeHandler;

  ros::Subscriber m_rosSubPolyLanes;

  ros::Publisher m_rosPubDrivingWay;

  double m_lookAhead_param = 0.0;
  double targetSpeed_ms = 0.0;
  double speedset = 0.0;
  double distance_bias = 0.0;

public:
  find_drivingway() {
    m_rosSubPolyLanes = m_rosNodeHandler.subscribe(
          "waypoint_polylanearr", 1000, &find_drivingway::polyLanesCallback, this);

    m_rosPubDrivingWay =
        m_rosNodeHandler.advertise<kusv_msgs::PolyfitLaneData>(
          "waypoint_lane", 1000);
  }

  ~find_drivingway() {}

protected:
  kusv_msgs::PolyfitLaneDataArray m_polyLanes;
  kusv_msgs::PolyfitLaneData m_midPolyLane;
  int count = 0;
  int lanecount = 0;

public:
  void polyLanesCallback(const kusv_msgs::PolyfitLaneDataArray::ConstPtr &msg) {
    m_polyLanes = *msg;
  }

  void findDrivingWay() {
    auto lanesize = m_polyLanes.polyfitLanes.size();
    //printf("%ld\n", lanesize);
    if (lanesize == 0) {
      m_midPolyLane.d = m_midPolyLane.d;
      m_midPolyLane.c = m_midPolyLane.c;
      m_midPolyLane.b = m_midPolyLane.b;
      m_midPolyLane.a = m_midPolyLane.a;
    }
    else if (lanesize >= 1) {
      float a[4*lanesize];
      for(int i = 0; i < lanesize; i++) {
        a[4*i]  = m_polyLanes.polyfitLanes[i].d;
        a[4*i+1]= m_polyLanes.polyfitLanes[i].c;
        a[4*i+2]= m_polyLanes.polyfitLanes[i].b;
        a[4*i+3]= m_polyLanes.polyfitLanes[i].a;
      }
      m_midPolyLane.d = a[0];
      m_midPolyLane.c = a[1];
      m_midPolyLane.b = a[2];
      m_midPolyLane.a = a[3];
    }
    m_midPolyLane.frame_id = "/body";
    m_rosPubDrivingWay.publish(m_midPolyLane);
  }
};


int main(int argc, char **argv) {
  ros::init(argc, argv, "waypoint_find_drivingway");

  find_drivingway find_drivingway;
  // The approximate control time is 100 Hz
  ros::Rate loop_rate(100);
  while (ros::ok()) {
    find_drivingway.findDrivingWay();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
