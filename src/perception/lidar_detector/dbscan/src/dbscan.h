#ifndef __DBSACN__
#define __DBSACN__

#include "cluster.h"

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>

#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"

#include <tf/tf.h>

typedef struct _rgb RGB;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloudXYZO;

struct _rgb
{
	uint8_t m_r;
	uint8_t m_g;
	uint8_t m_b;

	_rgb () 
	{ }

	_rgb (uint8_t r, uint8_t g, uint8_t b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}
};

extern ros::Publisher pub; // ros publisher    
extern std::vector<RGB> globalRGB; // store object color
extern int maxIndexNumber;

// Generate random color
void generateColor(size_t indexNumber);

// This function is called when subscribe "input"
void velodyne_callback (const sensor_msgs::PointCloud2ConstPtr &pInput);

// Downsampling
void voxelGrid (const PointCloudXYZO::ConstPtr& pInputCloud, PointCloudXYZO::Ptr pDownsampledCloud, float f_paramLeafSize_m);

void displayShape (const std::vector<clusterPtr> pVecClusters);
#endif //__DBSACN__
