#include "cluster.h"


Cluster::Cluster()
{
	m_valid_cluster = true;
}

pcl::PointCloud <pcl::PointXYZRGB>::Ptr Cluster::GetCloud()
{
	return m_pointCloud;
}

void Cluster::SetCloud(const pcl::PointCloud<pcl::PointXYZ>::Ptr pInputCloud,
		const std::vector<int>& vecClusterIndices, std_msgs::Header _header, int iClusterNumber, int r,
		int g, int b, std::string label)
{
	m_label = label;
	m_r = r;
	m_g = g;
	m_b = b;

	// extract pointcloud using the indices
	// calculate min and max points
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pCurrentCluster(new pcl::PointCloud<pcl::PointXYZRGB>);
	float min_x = std::numeric_limits<float>::max();
	float max_x = -std::numeric_limits<float>::max();
	float min_y = std::numeric_limits<float>::max();
	float max_y = -std::numeric_limits<float>::max();
	float min_z = std::numeric_limits<float>::max();
	float max_z = -std::numeric_limits<float>::max();

	for (auto pIt = vecClusterIndices.begin(); pIt != vecClusterIndices.end(); pIt++)
	{
		// fill new colored cluster point by point
		pcl::PointXYZRGB tmpPoint;
		tmpPoint.x = pInputCloud->points[*pIt].x;
		tmpPoint.y = pInputCloud->points[*pIt].y;
		tmpPoint.z = pInputCloud->points[*pIt].z;
		tmpPoint.r = r;
		tmpPoint.g = g;
		tmpPoint.b = b;

		pCurrentCluster->points.push_back(tmpPoint);

		if (tmpPoint.x > max_x)
			max_x = tmpPoint.x;
		if (tmpPoint.x < min_x)
			min_x = tmpPoint.x;
		if (tmpPoint.y > max_y)
			max_y = tmpPoint.y;
		if (tmpPoint.y < min_y)
			min_y = tmpPoint.y;
		if (tmpPoint.z > max_z)
			max_z = tmpPoint.z;
		if (tmpPoint.z < min_z)
			min_z = tmpPoint.z;
	}

	// min, max points
	m_min_point.x = min_x;
	m_max_point.x = max_x;
	m_min_point.y = min_y;
	m_max_point.y = max_y;
	m_min_point.z = min_z;
	m_max_point.z = max_z;

	// calculate bounding box
	m_length = m_max_point.x - m_min_point.x;
	m_width = m_max_point.y - m_min_point.y;
	m_height = m_max_point.z - m_min_point.z;

	//////////////////////////////////////////////////////////////////////////
	//									To do								//
	// Definition of a 'm_boundingBoxPose' and 'm_boundingBoxDimensions'	//
	// in cluster.h															//
	//																		//
	// 'm_boundingBoxPose' data type : geometry_msgs						//
	// 'm_boundingBoxDimensions' data type : geometry_msgs					//
	//																		//
	// Use m_minPoint.x and m_length to fill in m_boundingBoxPose.position.x//
	// Use m_minPoint.y and m_width to fill in m_boundingBoxPose.position.y	//
	// Use m_minPoint.z and m_height to fill in m_boundingBoxPose.position.z//
	//																		//
	// Use m_width fill in m_boundingBoxDimensions.y						// 
	// Use m_height fill in m_boundingBoxDimensions.z						//
	//////////////////////////////////////////////////////////////////////////
	m_boundingBoxPose.position.x = m_min_point.x + m_length/2 ;
        m_boundingBoxPose.position.y = m_min_point.y + m_width/2 ;
        m_boundingBoxPose.position.z = m_min_point.z + m_height/2 ;
	m_boundingBoxDimensions.x = ((m_length < 0) ? -1 * m_length : m_length);
        m_boundingBoxDimensions.y = ((m_width < 0) ? -1 * m_width : m_width);
        m_boundingBoxDimensions.z = ((m_height < 0) ? -1 * m_height : m_height);

	pCurrentCluster->width = pCurrentCluster->points.size();
	pCurrentCluster->height = 1;
	pCurrentCluster->is_dense = true;

	m_valid_cluster = true;
	m_pointCloud = pCurrentCluster;
}


Cluster::~Cluster()
{ }
