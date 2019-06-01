#include "dbscan.h"
#include "cluster.h"

#define BOX "box"

typedef pcl::PointCloud<pcl::PointXYZ> PointCloudXYZO;

ros::Publisher publishShape;  
ros::Publisher pub;	

std::vector<RGB> globalRGB; // store object color
int maxIndexNumber = 0;
std_msgs::Header velodyne_header;

std::string iShape = BOX;
float fMarkerDuration = 0.1f;
float fLeafSize_m = 0.1f;


int main (int argc, char** argv)
{
	srand ((unsigned int) time (NULL));

	// Initialize ROS
	ros::init (argc, argv, "euclidean_clustering");
	ros::NodeHandle nh;

	nh.setParam("shape_mode", iShape);
	nh.setParam("marker_duration", fMarkerDuration);
	nh.setParam("voxel_leafsize", fLeafSize_m);

	// Create a ROS subscriber for the input point cloud
	ros::Subscriber sub = nh.subscribe ("input", 1, velodyne_callback);

	// Create a Marker publisher for bounding box
	publishShape = nh.advertise<visualization_msgs::MarkerArray>("BoundingBox", 1);

	// Create a ROS publisher for the output point cloud
	pub = nh.advertise<PointCloudXYZO> ("output", 1);

	ros::Rate delay(10);

	while(ros::ok ())
	{
		ros::spinOnce();
		nh.getParam("shape_mode", iShape);
		nh.getParam("marker_duration", fMarkerDuration);
		nh.getParam("voxel_leafsize", fLeafSize_m);

		delay.sleep();
	}
}


// generate random color
void generateColor(size_t indexNumber)
{ 
	if (indexNumber > maxIndexNumber) 
	{ 
		int addRGB = indexNumber - maxIndexNumber; 
		maxIndexNumber = indexNumber; 

		for (size_t i = 0; i < addRGB; i++) 
		{ 
			uint8_t r = 1024 * rand () % 255; 
			uint8_t g = 1024 * rand () % 255; 
			uint8_t b = 1024 * rand () % 255; 
			globalRGB.push_back(RGB(r, g, b)); 
		}   
	} 
}   


// This function is called when subscribe "input"
void velodyne_callback (const sensor_msgs::PointCloud2ConstPtr &pInput)
{
	//////////////////////////////////////////////////////////////////
	// 1. Container for input data 
	PointCloudXYZO::Ptr pInputCloud (new PointCloudXYZO);
	pcl::fromROSMsg(*pInput, *pInputCloud);

	velodyne_header = pInput->header;

	//////////////////////////////////////////////////////////////////
	// 2. Create the filtering object: downsample the dataset using a leaf size
	PointCloudXYZO::Ptr pDownsampledCloud (new PointCloudXYZO);

	// Voxel length of the corner : fLeafSize_m
	pcl::VoxelGrid<pcl::PointXYZ> voxelFilter;
	voxelFilter.setInputCloud (pInputCloud);
	voxelFilter.setLeafSize(fLeafSize_m, fLeafSize_m, fLeafSize_m);
	voxelFilter.filter (*pDownsampledCloud);

	//////////////////////////////////////////////////////////////////
	// 3. DBSCAN

	// Creating the KdTree object for the search method of the extraction
	pcl::search::KdTree<pcl::PointXYZ>::Ptr pKdtreeDownsampledCloud (new pcl::search::KdTree<pcl::PointXYZ>);
	pKdtreeDownsampledCloud->setInputCloud (pDownsampledCloud);

	// DBSCAN object and parameter setting
	// Tolerance is the length from core point to query point
	// Min cluster size is the minimum number of points in the circle with the tolerance as radius 
	// Max cluster size is the maximum number of points in the circle with the tolerance as radius 
	// extract the index of each cluster to vecClusterIndices
	std::vector<pcl::PointIndices> vecClusterIndices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> euclideanCluster;
	euclideanCluster.setClusterTolerance (0.40);
	euclideanCluster.setMinClusterSize (20);
	euclideanCluster.setMaxClusterSize (10000);
	euclideanCluster.setSearchMethod (pKdtreeDownsampledCloud);
	euclideanCluster.setInputCloud (pDownsampledCloud);
	euclideanCluster.extract (vecClusterIndices);


	//////////////////////////////////////////////////////////////////
	// 4. Coloring and calculate the cluster center point
	
	// pVecClusters contain the colored local cluster.
	std::vector<clusterPtr> pVecClusters;

	int clusterNumber = 0;
	for (auto cluster_it = vecClusterIndices.begin (); cluster_it != vecClusterIndices.end (); ++cluster_it)
	{
		// generate random color to globalRGB variable
		generateColor(vecClusterIndices.size());

		// pCluster is a local cluster 
		clusterPtr pCluster (new Cluster());

		// Cloring and calculate the cluster center point and quaternion
		pCluster->SetCloud(pDownsampledCloud, cluster_it->indices, velodyne_header, clusterNumber, globalRGB[clusterNumber].m_r, globalRGB[clusterNumber].m_g, globalRGB[clusterNumber].m_b, "");
		pVecClusters.push_back(pCluster);

		clusterNumber++;
	}

	// Accumulate all cluster to pAccumulationCloud
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pAccumulationCloud (new pcl::PointCloud<pcl::PointXYZRGB>);
	pAccumulationCloud->header = pInputCloud->header;

	// publish
	for(auto cluster_it = pVecClusters.begin(); cluster_it != pVecClusters.end(); ++cluster_it)
	{
		*pAccumulationCloud += *((*cluster_it)->GetCloud());
	}

	displayShape(pVecClusters);
	pub.publish (*pAccumulationCloud);
}

// Display bounding box on each cluster
// Using the MarkerArray. Each member of MarkerArray is a bounding box for each cluster
void displayShape (const std::vector<clusterPtr> pVecClusters)
{
	visualization_msgs::MarkerArray arrShapes;
	arrShapes.markers.clear();

	uint32_t clusterNumber = 0;
	for (auto const& cluster_it : pVecClusters)
	{
		bool isClusterValid = cluster_it->m_valid_cluster;

		if (isClusterValid)
		{
			visualization_msgs::Marker shape;

			// Set marker life time and id.
			shape.lifetime = ros::Duration(fMarkerDuration);
			shape.header.frame_id = "velodyne";
			shape.id = clusterNumber;

			// set marker color
			shape.color.r = (float)cluster_it->m_r/255.0f;
			shape.color.g = (float)cluster_it->m_g/255.0f;
			shape.color.b = (float)cluster_it->m_b/255.0f;
			shape.color.a = 0.5;

			//////////////////////////////////////////////////////////////////////////
			//									To do								//
			// Declaration of cluster class in cluster.h							//
			// Definition of cluster class in cluster.cpp							//
			// 'cluster_it' data type : cluster(class) pointer						//
			// - Insert bounding box dimension that is member of 'cluster_it'		// 
			// into 'shape.scale'													//
			// - Insert bounding box position that is memeber of 'cluster_it'		//
			// into 'shape.pose.position											//
			//////////////////////////////////////////////////////////////////////////
			shape.type = visualization_msgs::Marker::CUBE;
			shape.action = visualization_msgs::Marker::ADD;
			shape.ns = "/boundingBox_markers";
			shape.scale = cluster_it->m_boundingBoxDimensions;
                        shape.pose.position = cluster_it->m_boundingBoxPose.position;


			arrShapes.markers.push_back(shape);
			clusterNumber++;
		}
	}

	publishShape.publish(arrShapes);
}
