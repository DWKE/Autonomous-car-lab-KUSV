/*---------------------------------------------------------------------------
** Includes
*/

// ROS
#include <ros/ros.h>

// ROS Messages
#include <sensor_msgs/NavSatFix.h>
#include "geometry_msgs/TwistWithCovarianceStamped.h"
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/NavSatFix.h"
#include "kusv_msgs/kusv_CanInfo.h"
#include "ublox_msgs/NavPVT.h" // ***Add aggm ,ublox_msgs and ublox_gps Package***
//#include "auto_interface/Motion.h" xxxxx
//#include "auto_interface/Gnss.h" xxxxx

// User define
#include "atom_task.h"  //** Modify Path!
#include "kusv_pose_interface.h"
#include "pose_estimation_ekf.hpp"

/*---------------------------------------------------------------------------
** Defines and Macros
*/


//---------------------------------------------------------------------------
// Typedefs and Functions
//

struct EstimatedLatitudeMsg{
	unsigned char sig_latitude_bf0:		8;
	unsigned char sig_latitude_bf1:		8;
	unsigned char sig_latitude_bf2:		8;
	unsigned char sig_latitude_bf3:		8;
	float 	dummy;
};
struct EstimatedLongitudeMsg{
	unsigned char sig_longitude_bf0:	8;
	unsigned char sig_longitude_bf1:	8;
	unsigned char sig_longitude_bf2:	8;
	unsigned char sig_longitude_bf3:	8;
	float 	dummy;
};
struct EstimatedHeadingMsg{
	unsigned char sig_heading_bf0:	8;
	unsigned char sig_heading_bf1:	8;
	unsigned char sig_heading_bf2:	8;
	unsigned char sig_heading_bf3:	8;
	float 	dummy;
};



class TaskNode : public AtomTask {
public:
	TaskNode(std::string task_node, double period); 
	~TaskNode();

public:
   	void Init();
	void init_pose_estimation_ekf(ros::Time pres_time);

	void Run();
	void Publish();
	void GetParameter();
	void Subscribe();
	void Terminate();

	// ======================================
	// ROS variables
	// publish
private:
	ros::Publisher m_pub_estimated_gnss;
	ros::Publisher m_pub_estimated_heading;
        ros::Publisher m_pub_gps_status;
        //ros::Publisher m_pub_gnss; xxxxx

	sensor_msgs::NavSatFix m_estimated_gnss;
	sensor_msgs::Imu m_estimated_heading;
        //auto_interface::Gnss auto_gnss_; xxxxx

	// publisher for can
	ros::Publisher m_pub_estimated_latitude_can;
	ros::Publisher m_pub_estimated_longitude_can;
	ros::Publisher m_pub_estimated_heading_can;
	

	EstimatedLatitudeMsg m_estimated_latitude_can;
	EstimatedLongitudeMsg m_estimated_longitude_can;
	EstimatedHeadingMsg m_estimated_heading_can;

	const int canmsg_id_estimated_latitude = 1000;
	const int canmsg_id_estimated_longitude = 1001;
	const int canmsg_id_estimated_heading = 1002;
	const int canmsg_dlc_msg = 8;

	const double LATITUDE_SCALE_FACTOR_CAN = 1e7;
	const double LONGITUDE_SCALE_FACTOR_CAN = 1e7;
	const double HEADING_SCALE_FACTOR_CAN = 1e5;

        const double kph2mps = 0.277777; //>>>
        const double acckph2mps = 0.0000771604; //>>>
        int lidar_odom_start = 0;
        int aaaa = 0;

	// subscribe
private:
	ros::Subscriber m_sub_motion;
	ros::Subscriber m_sub_gnss;
        ros::Subscriber m_sub_lidar_odom;
	
	// callback
private:
        void callback_motion(const kusv_msgs::kusv_CanInfo::ConstPtr& msg);
        void callback_gnss(const ublox_msgs::NavPVT::ConstPtr& msg);
        void callback_lidar_odom(const geometry_msgs::PoseStamped::ConstPtr& msg);

	// callback
private:

	// tf
private:
	void PublishEstimatedGnss();
	void MessageDisplay();
	// ======================================
	// algorithm
	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	// Inputs	
        KUSV_Motion	iMotion;
        KUSV_Gnss	iGnss;

	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	// Outputs

	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	// Properties
	std::string p_strInputMotion;
	std::string p_strInputGnss;
	std::string p_strOutputGnss;
	bool p_bIsDebugPoseEstimation;
	std::string p_ConfigPoseEstimationEkfFile;
	double p_EastOffset;
	double p_NorthOffset;
        double p_StopSpeed;
        double p_CanMsgCnt;
        double p_CanRate;

	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	// Variables
	ros::Time       motion_timestamp;
	ros::Time       gnss_timestamp;

	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
	// Mat matching
	CPoseEstimationEKF*	m_pPoseEstimationEKF;

};
