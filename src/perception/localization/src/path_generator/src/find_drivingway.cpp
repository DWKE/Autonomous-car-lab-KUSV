#include <ros/ros.h>
#include "std_msgs/Float64.h"
#include <math.h>
#include <ros/ros.h>
#include <string>
#include "path_generator/PolyfitLaneDataArray.h"
#include "path_generator/PolyfitLaneData.h"

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
          "waypoint_lane", 1000, &find_drivingway::polyLanesCallback, this);

    m_rosPubDrivingWay =
        m_rosNodeHandler.advertise<path_generator::PolyfitLaneData>(
          "driving_way", 1000);
  }

  ~find_drivingway() {}

protected:
  path_generator::PolyfitLaneDataArray m_polyLanes;
  path_generator::PolyfitLaneData m_midPolyLane;
  int count = 0;
  int lanecount = 0;

public:
  void polyLanesCallback(const path_generator::PolyfitLaneDataArray::ConstPtr &msg) {
    m_polyLanes = *msg;
  }

  void findDrivingWay() {
    auto lanesize = m_polyLanes.polyfitLanes.size();
    //printf("%ld\n", lanesize);
    if (lanesize == 0) {
      m_midPolyLane.a0 = m_midPolyLane.a0;
      m_midPolyLane.a1 = m_midPolyLane.a1;
      m_midPolyLane.a2 = m_midPolyLane.a2;
      m_midPolyLane.a3 = m_midPolyLane.a3;
    }
    else if (lanesize >= 1) {
      float a[4*lanesize];
      for(int i = 0; i < lanesize; i++) {
        a[4*i]  = m_polyLanes.polyfitLanes[i].a0;
        a[4*i+1]= m_polyLanes.polyfitLanes[i].a1;
        a[4*i+2]= m_polyLanes.polyfitLanes[i].a2;
        a[4*i+3]= m_polyLanes.polyfitLanes[i].a3;
      }
      m_midPolyLane.a0 = a[0];
      m_midPolyLane.a1 = a[1];
      m_midPolyLane.a2 = a[2];
      m_midPolyLane.a3 = a[3];
    }
    m_midPolyLane.frame_id = "/body";
    m_rosPubDrivingWay.publish(m_midPolyLane);
  }
};


int main(int argc, char **argv) {
  ros::init(argc, argv, "find_drivingway");

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
