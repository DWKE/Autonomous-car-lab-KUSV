#include "simulator.hpp"


Simulator::Simulator ()
{
	// publish m_shapes
	pub_simulatorShapes = nh.advertise <visualization_msgs::MarkerArray> ("sim_shape", 1);
	pub_origin = nh.advertise <visualization_msgs::Marker> ("origin", 1);

	// subscribe perception topic
	sub_object = nh.subscribe ("DetectedObject", 1000, &Simulator::detectedObjectArrayCallback, this);

	m_pListener = new TransformListener();

	tf::StampedTransform transform;
	m_pListener->waitForTransform (BASE_FRAME, LOCAL_FRAME, ros::Time(0), ros::Duration (10.0));
	m_pListener->lookupTransform (BASE_FRAME, LOCAL_FRAME, ros::Time(0), transform);

	origin();
}


Simulator::~Simulator ()
{}


void Simulator::mainLoop()
{
	ros::Rate rate(10.0);

	while (ros::ok())
	{
		ros::spinOnce();
		rate.sleep();
	}
}


void Simulator::origin()
{
	//origin
	m_Origin.header.frame_id = LOCAL_FRAME;
	m_Origin.header.stamp = ros::Time::now();

	m_Origin.ns = "/origin";
	m_Origin.id = 0;
	
	m_Origin.type = visualization_msgs::Marker::SPHERE;

	m_Origin.action = visualization_msgs::Marker::ADD;

	m_Origin.pose.position.x = 0;
	m_Origin.pose.position.y = 0;
	m_Origin.pose.position.z = 0;
	m_Origin.pose.orientation.x = 0.0;
	m_Origin.pose.orientation.y = 0.0;
	m_Origin.pose.orientation.z = 0.0;
	m_Origin.pose.orientation.w = 1.0;

	m_Origin.scale.x = 0.5;
	m_Origin.scale.y = 0.5;
	m_Origin.scale.z = 0.5;

	m_Origin.color.r = 1.0f;
	m_Origin.color.g = 0.0f;
	m_Origin.color.b = 0.0f;
	m_Origin.color.a = 1.0;

	m_Origin.lifetime = ros::Duration();
}


void Simulator::displayShape (const std::vector<std::vector<geometry_msgs::Point>> objects, string frame_)
{
	for (const auto& objectPoints : objects)
	{
		visualization_msgs::Marker shape;

		shape.lifetime = ros::Duration(0.1);
		shape.header.frame_id = frame_;
		shape.ns = frame_;
		shape.id = m_iObjectNum;

		if (frame_ == BASE_FRAME)
		{
			shape.color.r = 0.0f;
			shape.color.g = 1.0f; 
			shape.color.b = 0.0f;
			shape.color.a = 0.5;

		}
		else{
			shape.color.r = 1.0f;
			shape.color.g = 0.0f; 
			shape.color.b = 0.0f;
			shape.color.a = 0.5;
		}

		shape.type = visualization_msgs::Marker::LINE_STRIP;
		shape.action = visualization_msgs::Marker::ADD;
		shape.scale.x = 0.08;

		for (const auto &point : objectPoints)
		{
			geometry_msgs::Point tmpPoint;
			tmpPoint.x = point.x;
			tmpPoint.y = point.y;
			tmpPoint.z = point.z;

			shape.points.push_back (tmpPoint);
		}
		m_shapes.markers.push_back(shape);

		m_iObjectNum++;
	}
}

void Simulator::detectedObjectArrayCallback (const kusv_msgs::DetectedObjectArray::ConstPtr& input)
{
	// transformation point
	geometry_msgs::PointStamped laser_point;
	geometry_msgs::PointStamped base_point;
	laser_point.header.frame_id = BASE_FRAME;
	laser_point.header.stamp = ros::Time();

	std::vector<std::vector<geometry_msgs::Point>> velodyneObjectsPoints;
	std::vector<std::vector<geometry_msgs::Point>> transformVelodyneObjectsPoints;

	// transformation pose
	geometry_msgs::PoseStamped laser_pose;
	geometry_msgs::PoseStamped base_pose;
	laser_pose.header.frame_id = BASE_FRAME;
	laser_pose.header.stamp = ros::Time();

	std::vector<geometry_msgs::Pose> velodyneObjectsPose;
	std::vector<geometry_msgs::Pose> transformVelodyneObjectsPose;


	for (const auto& object : input->objects)
	{
		// transformation point
		std::vector<geometry_msgs::Point> velodyneObjectPoints;
		std::vector<geometry_msgs::Point> transformVelodyneObjectPoints;

		// transform point and push
		for (const auto& point : object.convex_hull)
		{
			laser_point.point.x = point.x;
			laser_point.point.y = point.y;
			laser_point.point.z = point.z;
			velodyneObjectPoints.push_back (laser_point.point);
			

			m_pListener->transformPoint(LOCAL_FRAME, laser_point, base_point);
			transformVelodyneObjectPoints.push_back (base_point.point);
		}

		velodyneObjectsPoints.push_back(velodyneObjectPoints);
		transformVelodyneObjectsPoints.push_back(transformVelodyneObjectPoints);

		// transform pose and push
		laser_pose.pose = object.pose;
		velodyneObjectsPose.push_back (laser_pose.pose);

		m_pListener->transformPose(LOCAL_FRAME, laser_pose, base_pose);
		transformVelodyneObjectsPose.push_back (base_pose.pose);
	}

	m_iObjectNum = 0;
	m_shapes.markers.clear();
	displayShape (velodyneObjectsPoints, laser_point.header.frame_id);
	displayShape (transformVelodyneObjectsPoints,base_point.header.frame_id);
	

	pub_simulatorShapes.publish (m_shapes);
	pub_origin.publish (m_Origin);
}
