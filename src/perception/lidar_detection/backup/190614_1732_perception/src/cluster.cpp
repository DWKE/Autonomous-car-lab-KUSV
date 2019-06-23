#include "cluster.hpp"

Cluster::Cluster()
{
	m_valid_cluster = true;
}

pcl::PointCloud <pcl::PointXYZRGB>::Ptr Cluster::GetCloud()
{
	return m_pointCloud;
}

void Cluster::SetCloud(const pcl::PointCloud<pcl::PointXYZI>::Ptr pInputCloud,
		const std::vector<int>& vecClusterIndices, std_msgs::Header _header, int iClusterNumber, int r,
		int g, int b, std::string label, bool doEstimatePose)
{
	m_id = iClusterNumber;
	m_originalID = iClusterNumber;

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
	float average_x = 0, average_y = 0, average_z = 0;


	for (auto indice : vecClusterIndices)
	{
		// fill new colored cluster point by point
		pcl::PointXYZRGB tmpPoint;
		tmpPoint.x = pInputCloud->points[indice].x;
		tmpPoint.y = pInputCloud->points[indice].y;
		tmpPoint.z = pInputCloud->points[indice].z;
		tmpPoint.r = r;
		tmpPoint.g = g;
		tmpPoint.b = b;

		pCurrentCluster->points.push_back(tmpPoint);

		if (tmpPoint.x < min_x)
			min_x = tmpPoint.x;
		if (tmpPoint.y < min_y)
			min_y = tmpPoint.y;
		if (tmpPoint.x > max_x)
			max_x = tmpPoint.x;
		if (tmpPoint.y > max_y)
			max_y = tmpPoint.y;
		if (tmpPoint.z > max_z)
			max_z = tmpPoint.z;
		if (tmpPoint.z < min_z)
			min_z = tmpPoint.z;
	}

	// min, max points
	m_min_point.x = min_x;
	m_min_point.y = min_y;
	m_min_point.z = min_z;
	m_max_point.x = max_x;
	m_max_point.y = max_y;
	m_max_point.z = max_z;

	// calculate bounding box
	m_height = m_max_point.z - m_min_point.z;

	m_center.position.z = m_min_point.z + m_height / 2;
	m_dimensions.z = ((m_height < 0) ? -1 * m_height : m_height);

	// pose estimation
	double rz = 0;

	{
		std::vector<cv::Point2f> points;
		for (unsigned int i = 0; i < pCurrentCluster->points.size(); i++)
		{
			cv::Point2f pt;
			pt.x = pCurrentCluster->points[i].x;
			pt.y = pCurrentCluster->points[i].y;
			points.push_back(pt);
		}

		std::vector<cv::Point2f> vecHull;
		cv::convexHull(points, vecHull);

		m_polygon.header = _header;
		for (size_t i = 0; i < vecHull.size() + 1; i++)
		{
			geometry_msgs::Point32 point;
			point.x = vecHull[i % vecHull.size()].x;
			point.y = vecHull[i % vecHull.size()].y;
			point.z = m_min_point.z;
			m_polygon.polygon.points.push_back(point);
		}

		for (size_t i = 0; i < vecHull.size() + 1; i++)
		{
			geometry_msgs::Point32 point;
			point.x = vecHull[i % vecHull.size()].x;
			point.y = vecHull[i % vecHull.size()].y;
			point.z = m_max_point.z;
			m_polygon.polygon.points.push_back(point);
		}

		if (doEstimatePose)
		{
			cv::RotatedRect box = minAreaRect(vecHull);
			rz = box.angle * 3.14 / 180;
			m_center.position.x = box.center.x;
			m_center.position.y = box.center.y;
			m_dimensions.x = box.size.width;
			m_dimensions.y = box.size.height;
		}

		// calculate bounding box
		m_length = m_dimensions.x;
		m_width = m_dimensions.y;
		m_height = m_dimensions.z;
	}

	// set bounding box direction
	tf::Quaternion quat = tf::createQuaternionFromRPY(0.0, 0.0, rz);
	tf::quaternionTFToMsg(quat, m_center.orientation);

	pCurrentCluster->width = pCurrentCluster->points.size();
	pCurrentCluster->height = 1;
	pCurrentCluster->is_dense = true;

	m_valid_cluster = true;
	m_pointCloud = pCurrentCluster;
}


Cluster::~Cluster()
{ }
