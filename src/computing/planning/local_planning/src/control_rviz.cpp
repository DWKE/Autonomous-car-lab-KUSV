#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include "kusv_msgs/PolyfitLaneData.h"

class Display{
protected:
    ros::NodeHandle m_rosNodeHandler;

    ros::Publisher m_rosPubDrivingWay;
    ros::Subscriber m_rosSubDrivingWay;

public:
    Display(){
        m_rosPubDrivingWay =
            m_rosNodeHandler.advertise<visualization_msgs::MarkerArray>(			// �̰� �ǵ���� ��
                "driving_way_marker", 10);
        m_rosSubDrivingWay = m_rosNodeHandler.subscribe(
            "driving_way", 10, &Display::drivingWayCallback, this);				// �̰� �ǵ���� ��
    }

      ~Display() {}


};
