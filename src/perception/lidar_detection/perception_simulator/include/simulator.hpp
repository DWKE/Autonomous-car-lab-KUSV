#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include "ros/ros.h"
#include <iostream>
#include <vector>
#include "kusv_msgs/DetectedObjectArray.h"
#include "visualization_msgs/MarkerArray.h"
#include <geometry_msgs/PointStamped.h>

#include <tf/transform_listener.h>

#define BASE_FRAME "velodyne"
#define LOCAL_FRAME "body"

using namespace std;
using namespace tf;


class Simulator
{
	private:
		ros::NodeHandle nh;

		// Publisher
		ros::Publisher pub_simulatorShapes;
		ros::Publisher pub_origin;

		// Subscriber
		ros::Subscriber sub_object;

		tf::TransformListener *m_pListener;
		visualization_msgs::MarkerArray m_shapes;

		uint32_t m_iObjectNum;
		visualization_msgs::Marker m_Origin;
	public:
		Simulator ();
		virtual ~Simulator();

		void mainLoop();
		void origin();
		void displayShape (const std::vector<std::vector<geometry_msgs::Point>> objects, string frame_);
		void detectedObjectArrayCallback (const kusv_msgs::DetectedObjectArray::ConstPtr& input);

};

#endif //__SIMULATOR_H__
