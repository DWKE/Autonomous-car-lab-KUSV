#ifndef __CLUSTER_H__
#define __CLUSTER_H__

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <geometry_msgs/PolygonStamped.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <tf/tf.h>

class Cluster
{
	pcl::PointCloud <pcl::PointXYZRGB>::Ptr m_pointCloud;

	public:
	pcl::PointXYZI m_min_point;
	pcl::PointXYZI m_max_point;
	float m_length, m_width, m_height;

	geometry_msgs::PolygonStamped m_polygon;
	geometry_msgs::Pose m_center;
	geometry_msgs::Vector3 m_dimensions;
	double m_dDistanceToCenter;

	std::string m_label;
	int m_id;
	int m_originalID;
	int m_r, m_g, m_b;

	Eigen::Matrix3f m_eigen_vectors;
	Eigen::Vector3f m_eigen_values;

	bool m_valid_cluster;
	void SetCloud(const pcl::PointCloud<pcl::PointXYZI>::Ptr pInputCloud,
			const std::vector<int>& vecClusterIndices, std_msgs::Header _header, int iClusterNumber, int r,
			int g, int b, std::string label, bool doEstimatePose);


	Cluster();
	virtual ~Cluster();
	/* \brief Sets whether the Cluster is valid or not*/
	void SetValidity(bool in_valid);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr GetCloud();
};

typedef boost::shared_ptr<Cluster> clusterPtr;

#endif //__CLUSTER_H__
