/*---------------------------------------------------------------------------
** Includes
*/

// ros
#include <tf/LinearMath/Matrix3x3.h>
#include <tf/LinearMath/Quaternion.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/NavSatFix.h>
#include <rosgraph_msgs/Clock.h>
#include <std_msgs/Float32.h>

// User define
#include "kusv_pose_estimation_ekf.hpp"
#include "geometry2d.h"
#include "ublox_gps/utils.h"
#include "mkgmtime.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ros_can/DR_data.h"

/*---------------------------------------------------------------------------
** Defines and Macros
*/

//---------------------------------------------------------------------------
// Typedefs and Functions
//

TaskNode::TaskNode(std::string task_node, double period)
    : AtomTask(task_node, period),
      m_pPoseEstimationEKF(NULL)
{

}

TaskNode::~TaskNode(){

}

void TaskNode::Init(){
    //----------------------------------------------------------
    // Initialization of the variables and input
    initMotion(&iMotion);
    initGnss(&iGnss);

    //----------------------------------------------------------
    // Get parameter
    GetParameter();

    //----------------------------------------------------------
    // Register subscriber
    Subscribe();

    //----------------------------------------------------------
    // Register Publisher
    ros::NodeHandle nh;
    m_pub_estimated_gnss = nh.advertise<sensor_msgs::NavSatFix>("pose_estimation/estimated_gnss", 10);
    m_pub_estimated_heading = nh.advertise<sensor_msgs::Imu>("pose_estimation/estimated_heading", 10);
    m_pub_estimated_state = nh.advertise<ublox_msgs::NavPVT>("pose_estimation/estimated_state", 10);
    m_pub_gps_status = nh.advertise<std_msgs::Float32>("pose_estimation/gps_status",10);
    m_pub_yawrate_bias = nh.advertise<std_msgs::Float32>("pose_estimation/yawrate_bias", 10);

}

void TaskNode::GetParameter(){
    ros::NodeHandle nh;
    ros::NodeHandle nhPrivate("~");

    // Parse parameters
    nhPrivate.getParam("p_strInputMotion",p_strInputMotion);
    nhPrivate.getParam("p_strInputGnss",p_strInputGnss);
    nhPrivate.getParam("p_strOutputGnss",p_strOutputGnss);
    nhPrivate.getParam("p_bIsDebugPoseEstimation",p_bIsDebugPoseEstimation);
    nhPrivate.getParam("p_ConfigPoseEstimationEkfFile",p_ConfigPoseEstimationEkfFile);
    nhPrivate.getParam("p_EastOffset",p_EastOffset);
    nhPrivate.getParam("p_NorthOffset",p_NorthOffset);
    nhPrivate.getParam("p_StopSpeed",p_StopSpeed);
    nhPrivate.getParam("p_CanMsgCnt",p_CanMsgCnt);

    ROS_INFO("p_ConfigPoseEstimationEkfFile : %s", p_ConfigPoseEstimationEkfFile.c_str());
}

void TaskNode::Subscribe(){
    ros::NodeHandle nh;

    // Set subscriber
    m_sub_motion = nh.subscribe(p_strInputMotion, 10, &TaskNode::callback_motion, this);
    m_sub_gnss = nh.subscribe(p_strInputGnss, 10, &TaskNode::callback_gnss, this); //>>>
    m_sub_lidar_odom = nh.subscribe("/lidar_odom",10, &TaskNode::callback_lidar_odom, this);

}


void TaskNode::init_pose_estimation_ekf(ros::Time pres_time)
{
    if(m_pPoseEstimationEKF != NULL){
        return;
    }


    //----------------------------------------------------------
    // Create the localization algorithm module
    m_pPoseEstimationEKF = new CPoseEstimationEKF;

    if (m_pPoseEstimationEKF == NULL) {
        ROS_ERROR("Fail to create the PoseEstimationEKF module"); // error report
    }

    // Initialization with INI file

    double pres_time_us = (double)pres_time.sec * 1000000. + (double)pres_time.nsec / 1000.;
    if (!m_pPoseEstimationEKF->Init((int64_t)pres_time_us, p_ConfigPoseEstimationEkfFile)) {
        ROS_ERROR("Initialization fail!"); // Reports this information to the RTMaps console
    }
}

void TaskNode::Run() {

    if (m_pPoseEstimationEKF == NULL) {
        ROS_INFO("EKF is not initialized");
        return;
    }

    if (m_pPoseEstimationEKF->RunPrediction() == false) {
        ROS_INFO("Run prediction fail");
        return;
    }

    // Run Pose update
    if (m_pPoseEstimationEKF->RunUpdate() == false) {
        ROS_INFO("Run update fail");
        return;
    }
}

void TaskNode::Publish(){
    if (m_pPoseEstimationEKF == NULL) {
        ROS_INFO("EKF is not initialized");
        return;
    }

    // Publish
    PublishEstimatedGnss();

    // Display
    if(p_bIsDebugPoseEstimation == true){
        MessageDisplay();
    }
}

void TaskNode::PublishEstimatedGnss(){
    if (m_pPoseEstimationEKF == NULL) {
        ROS_INFO("EKF is not initialized");
        return;
    }
    // Estimate of pose with the offset correction
    KUSV_Gnss OutputEstimatedGnss;
    double dEastOffset_m = p_EastOffset;
    double dNorthOffset_m = p_NorthOffset;
    if (abs(dEastOffset_m) < DBL_EPSILON && abs(dNorthOffset_m) < DBL_EPSILON) {
        OutputEstimatedGnss = m_pPoseEstimationEKF->GetEstimatedGnss();
    }
    else {
        OutputEstimatedGnss = m_pPoseEstimationEKF->GetEstimatedGnss(dEastOffset_m, dNorthOffset_m);
    }
    

    // Get time
    int64_t	nRepTimestamp = m_pPoseEstimationEKF->GetTimestampe();
    ros::Time repr_time;
    repr_time.sec = (int) ((double)nRepTimestamp / 1e6);
    repr_time.nsec = (int) (((double)nRepTimestamp - (double)repr_time.sec  * 1e6) * 1e3);

    ros::Time pres_time = ros::Time::now(); 

    //ROS_INFO("Repr Time : %f Pre Time : %f",repr_time.toSec(),pres_time.toSec());

    // Compensate position
    double diffTime = 0;
    double dDiffTime_sec = diffTime;
    //ros::Duration diffTime = pres_time-repr_time;
    //double dDiffTime_sec = diffTime.toSec();
    //ROS_INFO("pres_time[sec]  : %f",pres_time.toSec());
    //ROS_INFO("repr_time[sec]  : %f",repr_time.toSec());
    //ROS_INFO("DiffTIme [sec]  : %f",dDiffTime_sec);
    double dLon_m = iMotion.speed_x * dDiffTime_sec;
    double dDiffHdg_deg = iMotion.yaw_rate * dDiffTime_sec;

    double dHeading_deg = (double)OutputEstimatedGnss.heading;
    double dHeading_rad = dHeading_deg * M_PI / 180.;
    double dNextEast_m = dLon_m * cos(dHeading_rad + M_PI / 2.);
    double dNextNorth_m = dLon_m * sin(dHeading_rad + M_PI / 2.);
    double dNextHdg_deg = dHeading_deg + dDiffHdg_deg;
    double dNextHdg_rad = dNextHdg_deg * M_PI / 180.;

    // ENU to LLH
    GeoPoint2D	geoCurr = { OutputEstimatedGnss.latitude , OutputEstimatedGnss.longitude };
    Point2D	enuNext = { dNextEast_m, dNextNorth_m };
    GeoPoint2D	geoNext = { 0.,0. };
    cartesianToWgs84(&geoCurr, &enuNext, &geoNext);
    //ROS_INFO("OutputEstimatedGnssNext : [ %f // %f]", geoCurr.latitude, geoCurr.longitude); // skkim
    //ROS_INFO("OutputEstimatedGnssNext : [ %f // %f]", geoNext.latitude, geoNext.longitude); // skkim

    tf::Quaternion q;
    q.setRPY(0., 0., dNextHdg_rad + M_PI/2 );  // Publish north is zero  //>>>

    // Set data
    m_estimated_gnss.header.stamp.sec = pres_time.sec;
    m_estimated_gnss.header.stamp.nsec = pres_time.nsec;
    m_estimated_gnss.latitude = geoNext.latitude;
    m_estimated_gnss.longitude = geoNext.longitude;
    m_estimated_gnss.position_covariance[0] = (OutputEstimatedGnss.longitude_sigma)*(OutputEstimatedGnss.longitude_sigma); //>>> x
    m_estimated_gnss.position_covariance[4] = (OutputEstimatedGnss.latitude_sigma)*(OutputEstimatedGnss.latitude_sigma); //>>> y
    m_estimated_gnss.position_covariance[8] = 0; //>>> z

    m_estimated_heading.header.stamp.sec = pres_time.sec;
    m_estimated_heading.header.stamp.nsec = pres_time.nsec;
    m_estimated_heading.orientation.x = q.x();
    m_estimated_heading.orientation.y = q.y();
    m_estimated_heading.orientation.z = q.z();
    m_estimated_heading.orientation.w = q.w();

    // Set estimated_state data(gnss + heading integrated)
    m_estimated_state.lat = (int32_t)(geoNext.latitude  * 1e7);
    m_estimated_state.lon = (int32_t)(geoNext.longitude * 1e7);
    m_estimated_state.heading = (int32_t)((OutputEstimatedGnss.heading + 90.0)*1e5);

    // Publish
    m_pub_estimated_gnss.publish(m_estimated_gnss);
    m_pub_estimated_heading.publish(m_estimated_heading);
    m_pub_estimated_state.publish(m_estimated_state);
}

void TaskNode::Terminate(){
    if(m_pPoseEstimationEKF != NULL)
        delete m_pPoseEstimationEKF;
}

void TaskNode::callback_motion(const ros_can::DR_data::ConstPtr& msg){

    // ROS_INFO("callback_motion");

    static long long int motion_msg_cnt = 0; //>>>
    //static int imotion_avg_msg_cnt = 0;
    static double yaw_rate_bias, sum_yaw_rate = 0, sum_car_speed = 0;
    std_msgs::Float32 yaw_rate_bias_;

    // init map matching pf
    init_pose_estimation_ekf(msg->header.stamp);

    // Set motion data
    double motion_timestamp = (double)msg->header.stamp.sec * 1e6 + (double)msg->header.stamp.nsec / 1e3;
    iMotion.timestamp = (uint32_t)motion_timestamp;
    iMotion.speed_x = (float32_t)(msg->speed_avr_rear*kph2mps); //>>> rear average speed, km/h or m/s ??
    iMotion.speed_y = 0;
    iMotion.speed_z = 0;
    
    iMotion.acceleration_x = 0; //(float32_t)(msg->lon_acc_speed*acckph2mps); //>>> longitudinal acc speed??
    iMotion.acceleration_y = 0;
    iMotion.acceleration_z = 0;

    iMotion.roll_rate = 0;
    iMotion.pitch_rate = 0;
    iMotion.yaw_rate = (float32_t)(msg->yaw_rate) - yaw_rate_bias; //Stop Average!


    if(fabs(iMotion.speed_x) < p_StopSpeed) //>>> Stop Average!
    {
        if(motion_msg_cnt > p_CanMsgCnt)
        {
            yaw_rate_bias = sum_yaw_rate/(float)motion_msg_cnt;

        }

        sum_yaw_rate += (float32_t)(msg->yaw_rate);
        motion_msg_cnt++;

    }
    else
    {
        motion_msg_cnt = 0;
        sum_yaw_rate = 0;
    }

    //ROS_INFO("Speed : %f StopSpeed : %f Sum yaw rate : %f Motion_msg : %d ", iMotion.speed_x , p_StopSpeed,sum_yaw_rate,motion_msg_cnt);

    //    if(imotion_avg_msg_cnt == 4){

    //          iMotion.timestamp = (uint32_t)motion_timestamp;
    //          iMotion.yaw_rate = (sum_yaw_rate + (float32_t)msg->yaw_rate) / 5.;
    //          iMotion.speed_x = (float32_t)(msg->speed_avr_r * kph2mps);

    //          m_pPoseEstimationEKF->SetMotionInput((int64_t)motion_timestamp, iMotion);

    //          imotion_avg_msg_cnt = 0;
    //          sum_yaw_rate = 0;

    //          //ROS_INFO("avg_yaw : %f avg_speed : %f",iMotion.yaw_rate, iMotion.speed_x);

    //    }else{

    //        sum_yaw_rate += (float32_t)msg->yaw_rate;
    //        imotion_avg_msg_cnt++;
    //    }

    std::ofstream out("/home/kusvcom2/motion.txt",std::ios::app);
    if(out.is_open()){
        out.precision(20);
        out<< msg->header.stamp.toSec() <<" "<<iMotion.speed_x <<" " <<iMotion.yaw_rate<<std::endl;

    }

    ROS_INFO("Speed : %f YawRate : %f YawRateBias : %f",iMotion.speed_x,iMotion.yaw_rate,yaw_rate_bias);

    // Motion update
    m_pPoseEstimationEKF->SetMotionInput((int64_t)motion_timestamp, iMotion); //>>> Stop Average!

    // debug for yaw rate bias
    yaw_rate_bias_.data = yaw_rate_bias;
    m_pub_yawrate_bias.publish(yaw_rate_bias_);
}


void TaskNode::callback_gnss(const ublox_msgs::NavPVT::ConstPtr& msg){
    // init map matching pf

    //ROS_INFO("callback_gnss");

    if(((float32_t)(msg->hAcc) / 1000.f) < 0.5){

        aaaa++;
        lidar_odom_start = 0;

        ros::Time gps_time_; //>>>

        std_msgs::Float32 gps_status_; //>>>

        gps_time_.sec = toUtcSeconds(*msg); //>>>
        gps_time_.nsec = msg->nano; //>>>


        init_pose_estimation_ekf(gps_time_);

        // Get time
        ros::Time repr_time = gps_time_;
        double gpstime = gps_time_.toSec();
        ros::Time pres_time = ros::Time::now();

        //ROS_INFO("Repr Time : %f Pre Time : %f",repr_time.toSec(),pres_time.toSec());

        // Get message information
        float64_t dMsgGnssLatitude = (float64_t)(msg->lat) * 1e-7;
        float64_t dMsgGnssLongitude = (float64_t)(msg->lon) * 1e-7;
        float32_t fMsgGnssHorizontalAccuracy_m = (float32_t)(msg->hAcc) / 1000.f;
        float32_t fMsgGnssHeadingAccuracy_deg = (float32_t)(msg->headAcc)*1e-5f;
        int iMsgGnssFlags  = (int)(msg->flags);
        uint32_t iMsgGnssNumSat = (uint32_t)(msg->numSV);
        float32_t fMsgGnsspDop = (float32_t)(msg->pDOP);
        double dMsgGnssHeading = (double)(msg->heading)*1e-5;

        // Compensate position
        double diffTime = 0;//(pres_time-repr_time).toSec(); //>>>

        double dDiffTime_sec = diffTime;
        double dLon_m = iMotion.speed_x * dDiffTime_sec;
        double dDiffHdg_deg = iMotion.yaw_rate * dDiffTime_sec;

        double dHeading_deg = - dMsgGnssHeading;  // >>> Check First
        double dHeading_rad = dHeading_deg * M_PI / 180.;
        double dNextEast_m = dLon_m * cos(dHeading_rad + M_PI / 2.);
        double dNextNorth_m = dLon_m * sin(dHeading_rad + M_PI / 2.);
        double dNextHdg_deg = dHeading_deg + dDiffHdg_deg;
        double dNextHdg_rad = dNextHdg_deg * M_PI / 180.;

        // ENU to LLH
        GeoPoint2D	geoCurr = { dMsgGnssLatitude, dMsgGnssLongitude };
        Point2D	enuNext = { dNextEast_m, dNextNorth_m };
        GeoPoint2D	geoNext = { 0.,0. };
        cartesianToWgs84(&geoCurr, &enuNext, &geoNext);

        // Set Gnss measurement
        double gnss_timestamp = (double)pres_time.sec * 1e6 + (double)pres_time.nsec / 1e3;
        iGnss.timestamp = (uint32_t)gnss_timestamp;
        iGnss.latitude = (float64_t)geoNext.latitude;
        iGnss.longitude = (float64_t)geoNext.longitude;
        iGnss.heading = (float32_t)dNextHdg_deg;
        iGnss.latitude_sigma = fMsgGnssHorizontalAccuracy_m;
        iGnss.longitude_sigma = fMsgGnssHorizontalAccuracy_m;
        iGnss.heading_sigma = fMsgGnssHeadingAccuracy_deg;

        ROS_INFO("fMsgGnssHorizontalAccuracy_m : %f",fMsgGnssHorizontalAccuracy_m);

        if(fMsgGnssHorizontalAccuracy_m < 0.25){
            gps_status_.data = 4;
        }
        else if(fMsgGnssHorizontalAccuracy_m < 0.5)
        {
            gps_status_.data = 5;
        }
        else
        {
            gps_status_.data = 0;
        }

        //        switch (iMsgGnssFlags) //>>>
        //        {
        //        case 0 :
        //            gps_status_.data = 0; //No Gps
        //            break;
        //        case 1 :
        //            gps_status_.data = 1; //No Solution
        //            break;
        //        case 3 :
        //            gps_status_.data = 2; //DGPS Used
        //            break;
        //        case 67 :
        //            gps_status_.data = 5; // Float
        //            break;
        //        case 131 :
        //            gps_status_.data = 4; // Fix
        //            break;
        //        default :
        //            ROS_INFO("NO GPS_STATUS");
        //            gps_status_.data = 0;

        //            break;
        //        }

        iGnss.quality = (KUSV_GNSS_QUALITY)(gps_status_.data);

        iGnss.number_of_satellites = iMsgGnssNumSat;
        iGnss.HDOP = fMsgGnsspDop;

        m_pub_gps_status.publish(gps_status_);

        ROS_INFO("Gps : %f %f GpsHeading : %f Num Nav : %d dHeadingDeg : %f Diff_Time : %f",
                 iGnss.latitude,
                 iGnss.longitude,
                 iGnss.heading,
                 iMsgGnssNumSat,
                 dHeading_deg,
                 diffTime);

        std::ofstream out("/home/kusvcom2/gps_information.txt",std::ios::app);
        if(out.is_open()){

            out.precision(20);
            out<<gpstime<<" "<<iGnss.latitude <<" "<<iGnss.latitude_sigma<<" "<<iGnss.longitude<<" "
               <<iGnss.longitude_sigma<<" "<<msg->height*1e-3<<" "<<iGnss.heading<<" "<<fMsgGnssHeadingAccuracy_deg<<" "
               <<msg->gSpeed*1e-3<<" "<<msg->sAcc*1e-3<<" "<<iGnss.number_of_satellites<<" "<<iGnss.HDOP<<" "<<iMsgGnssFlags<<std::endl;

        }



        // Update measurements

        ROS_INFO("Update GPS");

        if (m_pPoseEstimationEKF->SetMeasGNSS((int64_t)gnss_timestamp, iGnss) == false){
            ROS_INFO("Fail to GNSS update!");
        }
    }else{

        //iGnss.quality = (KUSV_GNSS_QUALITY)0;
        lidar_odom_start = 1;
        ROS_INFO("No Update GPS");
    }

}

void TaskNode::callback_lidar_odom(const geometry_msgs::PoseStamped::ConstPtr& msg){

    if(lidar_odom_start == 1)
    {

        ros::Time repr_time = msg->header.stamp;
        ros::Time pres_time = ros::Time::now();

        double diffTime = (pres_time-repr_time).toSec(); //>>>

        ROS_INFO("Callback_Lidar_Odom : %f",diffTime);
        double dDiffTime_sec = diffTime;
        double dLon_m = iMotion.speed_x * dDiffTime_sec;
        double dDiffHdg_deg = iMotion.yaw_rate * dDiffTime_sec;
        double dDiffHdg_rad = dDiffHdg_deg * M_PI/180;

        tf::Quaternion q_(0, 0, msg->pose.orientation.z, msg->pose.orientation.w); //Be Careful when calculating yaw
        tf::Matrix3x3 m(q_);
        double roll, pitch, yaw;
        m.getRPY(roll, pitch, yaw);

        double dNextEast_m = dLon_m * cos( yaw + dDiffHdg_rad / 2 ) + msg->pose.position.x;
        double dNextNorth_m = dLon_m * sin( yaw + dDiffHdg_rad / 2 ) + msg->pose.position.y;
        double dNextHdg_deg = ( yaw + dDiffHdg_rad / 2 - M_PI / 2 ) * 180/M_PI;

        GeoPoint2D	geoCurr = { 35.8349389, 128.6811557 }; //For Daegu
        //GeoPoint2D	geoCurr = { 37.540032, 127.0709544 }; //For Konkuk
        Point2D	enuNext = { dNextEast_m, dNextNorth_m };
        GeoPoint2D	geoNext = { 0.,0. };
        cartesianToWgs84(&geoCurr, &enuNext, &geoNext);
        double gnss_timestamp = (double)pres_time.sec * 1e6 + (double)pres_time.nsec / 1e3;

        iGnss.timestamp = (uint32_t)gnss_timestamp;
        iGnss.latitude = (float64_t)geoNext.latitude;
        iGnss.longitude = (float64_t)geoNext.longitude;
        iGnss.heading = (float32_t)dNextHdg_deg;
        iGnss.latitude_sigma = 0.0001;
        iGnss.longitude_sigma = 0.0001;
        iGnss.heading_sigma = 0.0001;
        iGnss.quality = (KUSV_GNSS_QUALITY)4;

        ROS_INFO("Update Lidar Odom");
        ROS_INFO("Lidar : %f %f LidarHeading : %f Diff_Time : %f",
                 iGnss.latitude,
                 iGnss.longitude,
                 iGnss.heading,
                 diffTime);

        // Update measurements
        if (m_pPoseEstimationEKF->SetMeasGNSS((int64_t)gnss_timestamp, iGnss) == false){
            ROS_INFO("Fail to Lidar Odom update!");
        }

    }else{

        lidar_odom_start = 0;

    }

}


void TaskNode::MessageDisplay(){       
    // Message display from algorithm module
    std::string strDebuggingInfo;
    m_pPoseEstimationEKF->PopDebuggingInfo(strDebuggingInfo);
    if (strDebuggingInfo.size() > 0) {
        ROS_INFO("%s", strDebuggingInfo.c_str());
    }
}

int main(int argc, char **argv){
    std::string node_name = "pose_estimation_ekf";
    ros::init( argc, argv, node_name);
    ros::NodeHandle nh;
    ros::NodeHandle nhPrivate("~");

    TaskNode main_task = TaskNode("pose_estimation_ekf", 0.02);
    main_task.Exec();

    return 0;
}
