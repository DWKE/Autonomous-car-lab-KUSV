#ifndef __OBSTACLE_TRACKING_H__
#define __OBSTACLE_TRACKING_H__

#include <ros/ros.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <pcl_ros/point_cloud.h>

#include "cluster.hpp"

class ObstacleTracking
{
	public:
	std::vector<clusterPtr> m_DetectedObjects;
	std::vector<clusterPtr> m_TrackingObjects;
	std::vector<clusterPtr> m_TmpTrackingObjects;
	double m_MAX_ASSOCIATION_DISTANCE;
	unsigned int iTracksNumber;

	public:
	ObstacleTracking();
	~ObstacleTracking();
	//void doOneStep (const std::vector<clusterPtr>& objList);
	void associate (const std::vector<clusterPtr>& objList);
	void matchWithDistanceOnly ();
	void MergeObjectAndTrack (clusterPtr& track, clusterPtr& obj);
};


#endif //__OBSTACLE_TRACKING_H__
