/**
 * @copyright (c) Valeo Driving Assistance Research.
 * <br>All rights reserved. Subject to limited distribution and restricted disclosure only.
 * @author kichun.jo@valeo.com
 * @file pose_ekf.cpp
 * @brief Pose estimation EKF source file
 * @version 1.0
 * @date 15-09-2016
 * @bug	No known bugs
 * @warning	No warnings
 */

#include "pose_estimation_ekf.hpp"	// Includes the header of this component

#define _USE_MATH_DEFINES
#include <math.h>
#include <ros/ros.h>

#include "geometry2d.h"  //***Modify Path!!

 // For debug
#include <cstdio>
#include <iostream>
#include <iomanip>


 //##########################################################################################
 // 00. Management

CPoseEstimationEKF::CPoseEstimationEKF()
	:
	// 00. Management
	m_bFlagInitComplete(false),
	m_nTimestampe_us(0),
	m_bPrediction(true),
	m_bUpdate(true),
	// 01. Fault detection
	// 02. prediction
	m_nInputMotionTimeStamp_us(0),
	m_bFlagOfResetForInitPrediction(true),
	// 03. Measurement update	
	m_nLatestUpdateTimestamp(0),
	m_nMeasGnssTimestamp(0),
	m_dGnssUncertaintyCoef_lat(0.),
	m_dGnssUncertaintyCoef_lat_v(0.),
	m_dGnssUncertaintyCoef_lon(0.),
	m_dGnssUncertaintyCoef_lon_v(0.),
	m_dGnssUncertaintyCoef_h(0.),
	m_dGnssUncertaintyCoef_h_w(0.),
	// 99. Miscellaneous
	// Configuration
	m_cfg_bDebugInfo(true),
	m_cfg_dInitailLatitude_deg(0.0),
	m_cfg_dInitailLongitude_deg(0.0),
	m_cfg_dInitailHeading_deg(0.0),
	m_cfg_dInitStateUncertainty_lat_m(0.0),
	m_cfg_dInitStateUncertainty_lon_m(0.0),
	m_cfg_dInitStateUncertainty_heading_deg(0.0),
	m_cfg_bRunPredictionForSpeedLimit(false),
	m_cfg_dRunPredictionSpeedLimit_kph(0.0),
	m_cfg_bRunUpdateForTimestampVariation(false),
	m_cfg_bRunUpdateForSpeedLimit(false),
	m_cfg_dRunUpdateSpeedLimit_kph(0.),
	m_cfg_bRunUpdateForGnssQuality(false),
	m_cfg_bRunUpdateForGnssNumOfSatLimit(false),
	m_cfg_iRunUpdateGnssNumOfSatLimit(0),
	m_cfg_bOnFaultDetectionBasedOnGnss(true),
	m_cfg_bOnGnssValidityCheckBasedOnStd(true),
	m_cfg_dGnssValidityStdThreshold_m(0.0),
	m_cfg_bOnGnssValidityCheckBasedOnQuality(true),
	m_cfg_bOnGnssValidityCheckBasedOnNumOfSat(true),
	m_cfg_dFaultPositionThreshold_m(0.0),
	m_cfg_dFaultHeadingThreshold_deg(0.0),
	m_cfg_dStateUncertaintyCoef_lat(1.0),
	m_cfg_dStateUncertaintyCoef_lat_v(0.0),
	m_cfg_dStateUncertaintyCoef_lat_a(0.0),
	m_cfg_dStateUncertaintyCoef_lon(1.0),
	m_cfg_dStateUncertaintyCoef_lon_v(0.0),
	m_cfg_dStateUncertaintyCoef_lon_a(0.0),
	m_cfg_dStateUncertaintyCoef_h(1.0),
	m_cfg_dStateUncertaintyCoef_h_w(0.0),
	m_cfg_bGnssUpdate_Q0(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q0(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q0(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q0(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q0(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q0(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q0(0.0),
	m_cfg_bGnssUpdate_Q1(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q1(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q1(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q1(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q1(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q1(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q1(0.0),
	m_cfg_bGnssUpdate_Q2(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q2(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q2(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q2(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q2(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q2(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q2(0.0),
	m_cfg_bGnssUpdate_Q3(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q3(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q3(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q3(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q3(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q3(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q3(0.0),
	m_cfg_bGnssUpdate_Q4(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q4(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q4(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q4(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q4(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q4(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q4(0.0),
	m_cfg_bGnssUpdate_Q5(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q5(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q5(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q5(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q5(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q5(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q5(0.0),
	m_cfg_bGnssUpdate_Q6(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q6(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q6(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q6(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q6(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q6(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q6(0.0),
	m_cfg_bGnssUpdate_Q7(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q7(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q7(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q7(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q7(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q7(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q7(0.0),
	m_cfg_bGnssUpdate_Q8(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q8(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q8(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q8(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q8(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q8(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q8(0.0),
	m_cfg_bGnssUpdate_Q9(false),
	m_cfg_dGnssUncertaintyCoef_lat_Q9(0.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Q9(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Q9(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Q9(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Q9(0.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Q9(0.0),
	m_cfg_bGnssUpdate_Def(false),
	m_cfg_dGnssUncertaintyCoef_lat_Def(1.0),
	m_cfg_dGnssUncertaintyCoef_lat_v_Def(0.0),
	m_cfg_dGnssUncertaintyCoef_lon_Def(1.0),
	m_cfg_dGnssUncertaintyCoef_lon_v_Def(0.0),
	m_cfg_dGnssUncertaintyCoef_h_Def(1.0),
	m_cfg_dGnssUncertaintyCoef_h_w_Def(0.0)
{
        memset(&m_AUTOMotion, 0, sizeof(KUSV_Motion));
        memset(&m_AUTOMotionPrev, 0, sizeof(KUSV_Motion));
	memset(&m_MeasGnss, 0, sizeof(m_MeasGnss));
	PutDebuggingInfo("Construction of CPoseEstimationEKF!");
}
CPoseEstimationEKF::~CPoseEstimationEKF()
{
	// 00. Management
	// 01. Fault detection
	// 03. Measurement update

	PutDebuggingInfo("Terminate CPoseEstimationEKF!");
}
bool CPoseEstimationEKF::Init(int64_t nTimestamp, std::string strCfgINIfilePath)
{
	m_bFlagInitComplete = false; // flag for reset and init

	// -----------------------------------------------------------------------
	// Parsing the INI configuration 
	if (strCfgINIfilePath.length() <= 0)
		return false;
	m_ini_parser.Init(strCfgINIfilePath);
	if (ParseConfigINIFile() != true){
		return false;
	}

	// -----------------------------------------------------------------------
	// variables initialization
	if (STATE_ORDER <= 0) return false;

	m_nTimestampe_us = nTimestamp;
	double dLenOfDegLat = lengthOfDegreeLatitude(m_cfg_dInitailLatitude_deg);
	double dLenOfDegLon = lengthOfDegreeLongitude(m_cfg_dInitailLatitude_deg);
	if (abs(dLenOfDegLat) <= 0. || abs(dLenOfDegLon) <= 0.)
		return false;

	// 00. Management			

	// State initialization using configuration	
	m_matX.create(STATE_ORDER, 1, CV_64FC1);
	m_matX = cv::Scalar(0.);
	m_matX.at<double>(0, 0) = m_cfg_dInitailLatitude_deg;
	m_matX.at<double>(1, 0) = m_cfg_dInitailLongitude_deg;
	m_matX.at<double>(2, 0) = m_cfg_dInitailHeading_deg;

	// Covariance initialization using configuration	
	m_matP.create(STATE_ORDER, STATE_ORDER, CV_64FC1);
	m_matP = cv::Scalar(0.);
	m_matP.at<double>(0, 0) = pow(m_cfg_dInitStateUncertainty_lat_m / dLenOfDegLat, 2.);
	m_matP.at<double>(1, 1) = pow(m_cfg_dInitStateUncertainty_lon_m / dLenOfDegLon, 2.);
	m_matP.at<double>(2, 2) = pow(m_cfg_dInitStateUncertainty_heading_deg, 2.);

	// 01. Fault detection & reset	

	// 03. Measurement update
	m_bFlagInitComplete = true;	// flag for reset and init

	// Debugging information
	PutDebuggingInfo("Initialization of Pose Estimation EKF!");

	return true;
}
double CPoseEstimationEKF::GetState(int nRow)
{
	if (nRow >= 0 && nRow < STATE_ORDER)
	{
		return m_matX.at<double>(nRow, 0);
	}
	else
	{
		return 0.0;
	}
}
double CPoseEstimationEKF::GetCovariance(int nRow, int nCol)
{
	if (nRow >= 0 && nRow < STATE_ORDER
		&& nCol >= 0 && nCol < STATE_ORDER)
	{
		return m_matP.at<double>(nRow, nCol);
	}
	else
	{
		return 0.0;
	}
}
KUSV_Gnss	CPoseEstimationEKF::GetEstimatedGnss(void)
{
        KUSV_Gnss GnssEstimated;

	double dLenOfDegLat = lengthOfDegreeLatitude(m_matX.at<double>(0, 0));
	double dLenOfDegLon = lengthOfDegreeLongitude(m_matX.at<double>(0, 0));

        memset(&GnssEstimated, 0, sizeof(KUSV_Gnss));
	GnssEstimated.latitude = m_matX.at<double>(0, 0);
	GnssEstimated.longitude = m_matX.at<double>(1, 0);
	GnssEstimated.altitude = (float32_t)0.f;
	GnssEstimated.heading = (float32_t)NormAngle_deg(m_matX.at<double>(2, 0) - 90.); // State heading: East is Zero / Gnss heading: North is zero
	GnssEstimated.latitude_sigma = (float32_t)(sqrt(m_matP.at<double>(0, 0))*dLenOfDegLat);
	GnssEstimated.longitude_sigma = (float32_t)(sqrt(m_matP.at<double>(1, 1))*dLenOfDegLon);
	GnssEstimated.altitude_sigma = (float32_t)0.f;
	GnssEstimated.heading_sigma = (float32_t)sqrt(m_matP.at<double>(2, 2));
	GnssEstimated.quality = m_MeasGnss.quality;

	GnssEstimated.number_of_satellites = m_MeasGnss.number_of_satellites;
	GnssEstimated.HDOP = m_MeasGnss.HDOP;

	GnssEstimated.timestamp = (uint32_t)(m_nTimestampe_us / 1000.);

	return GnssEstimated;
}
KUSV_Gnss	CPoseEstimationEKF::GetEstimatedGnss(double dEastOffset_m, double dNorthOffset_m)
{
	// Offset correction
	GeoPoint2D	geopointState = { m_matX.at<double>(0, 0) , m_matX.at<double>(1, 0) };
	Point2D		enupointCorrection = { dEastOffset_m, dNorthOffset_m };
	GeoPoint2D	geopointCorrection = { 0.,0. };
	cartesianToWgs84(&geopointState, &enupointCorrection, &geopointCorrection);


	// Return 
        KUSV_Gnss GnssEstimated;

	double dLenOfDegLat = lengthOfDegreeLatitude(geopointCorrection.latitude);
	double dLenOfDegLon = lengthOfDegreeLongitude(geopointCorrection.latitude);

        memset(&GnssEstimated, 0, sizeof(KUSV_Gnss));
	GnssEstimated.latitude = geopointCorrection.latitude;
	GnssEstimated.longitude = geopointCorrection.longitude;
	GnssEstimated.altitude = (float32_t)0.f;
	GnssEstimated.heading = (float32_t)NormAngle_deg(m_matX.at<double>(2, 0) - 90.); // State heading: East is Zero / Gnss heading: North is zero
	GnssEstimated.latitude_sigma = (float32_t)(sqrt(m_matP.at<double>(0, 0))*dLenOfDegLat);
	GnssEstimated.longitude_sigma = (float32_t)(sqrt(m_matP.at<double>(1, 1))*dLenOfDegLon);
	GnssEstimated.altitude_sigma = (float32_t)0.f;
	GnssEstimated.heading_sigma = (float32_t)sqrt(m_matP.at<double>(2, 2));
	if (m_bPrediction == true && m_bUpdate == false)
	{
                GnssEstimated.quality = KUSV_DR;
	}
	else
	{
		GnssEstimated.quality = m_MeasGnss.quality;
	}
	GnssEstimated.number_of_satellites = m_MeasGnss.number_of_satellites;
	GnssEstimated.HDOP = m_MeasGnss.HDOP;

	GnssEstimated.timestamp = (uint32_t)(m_nTimestampe_us / 1000.);

	return GnssEstimated;
}
bool CPoseEstimationEKF::RunPrediction(void)
{
	// -----------------------------------------------------------------------
	// In order to prevent running before initialization
	if (m_bFlagInitComplete != true)
		return false;

	// -----------------------------------------------------------------------
	// Update the configuration
	if (m_ini_parser.IsFileUpdated()){
		if(ParseConfigINIFile() != true){
			return false;
		}
		std::cout << "pose_estimation_ekf_map_matching.ini file is updated!" << std::endl;
	}

	// -----------------------------------------------------------------------
	// Decision of Prediction Mode
	bool	bPrediction = true;

	// 1-a. Speed based prediction decision
	if (m_cfg_bRunPredictionForSpeedLimit == true)
	{
                if (fabs(m_AUTOMotion.speed_x) < (m_cfg_dRunPredictionSpeedLimit_kph / 3.6))	// Stop
		{
                        //ROS_INFO("Predic False  Speed : %f %f", fabs(m_AUTOMotion.speed_x), (float)(m_cfg_dRunPredictionSpeedLimit_kph / 3.6));
			bPrediction = bPrediction && false;
		}
	}
	m_bPrediction = bPrediction;

	// -----------------------------------------------------------------------
	// Execution of Prediction 

	// flag for reset and init
	if (m_bFlagOfResetForInitPrediction == true)
	{
		// Save previous inputs
		m_nTimestampe_us = m_nInputMotionTimeStamp_us;
		m_AUTOMotionPrev = m_AUTOMotion;

		m_nLatestUpdateTimestamp = m_nMeasGnssTimestamp;
		m_bFlagOfResetForInitPrediction = false;	// Reset flag

		bPrediction = false;
	}

	// Run prediction
	bool bRunPrediction = false;
	if (bPrediction == true)
	{
		bRunPrediction = PredictionOneStep();
	}

	// Set the new timestamp and save the previous inputs
	m_nTimestampe_us = m_nInputMotionTimeStamp_us;
	m_AUTOMotionPrev = m_AUTOMotion;

	return true;
}
bool CPoseEstimationEKF::RunUpdate(void)
{
	// -----------------------------------------------------------------------
	// In order to prevent running before initialization
	if (m_bFlagInitComplete != true)
		return false;

	// -----------------------------------------------------------------------
	// Decision of Update Mode
	bool	bUpdate = true;

	// 2-a. Time stamp based update decision
	if (m_cfg_bRunUpdateForTimestampVariation == true)
	{
		if (m_nMeasGnssTimestamp <= m_nLatestUpdateTimestamp)	// No new measurement
		{
			bUpdate = bUpdate && false;
		}
	}

	// 2-b. Speed based update decision
	if (m_cfg_bRunUpdateForSpeedLimit == true)
	{
		if (abs(m_AUTOMotion.speed_x) < abs(m_cfg_dRunUpdateSpeedLimit_kph / 3.6))	// if not too slow
		{
			bUpdate = bUpdate && false;
		}
	}

	// 2-c. GNSS status based update based on the quality
	if (m_cfg_bRunUpdateForGnssQuality == true)
	{
		switch (m_MeasGnss.quality)	// GNSS Quality
		{
                case 0: // KUSV_NO_FIX
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q0;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q0;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q0;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q0;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q0;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q0;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q0;
			break;

                case 1: // KUSV_FIX
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q1;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q1;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q1;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q1;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q1;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q1;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q1;
			break;

                case 2: // KUSV_DIFF_FIX
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q2;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q2;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q2;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q2;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q2;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q2;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q2;
			break;

                case 3: // KUSV_PPS_FIX
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q3;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q3;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q3;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q3;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q3;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q3;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q3;
			break;

                case 4: // KUSV_RTK_INT
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q4;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q4;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q4;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q4;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q4;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q4;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q4;
			break;

                case 5: // KUSV_RTK_FLOAT
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q5;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q5;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q5;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q5;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q5;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q5;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q5;
			break;

                case 6: // KUSV_DR
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q6;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q6;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q6;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q6;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q6;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q6;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q6;
			break;

                case 7: // KUSV_MANUAL
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q7;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q7;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q7;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q7;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q7;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q7;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q7;
			break;

                case 8: // KUSV_SIMULATION
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q8;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q8;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q8;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q8;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q8;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q8;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q8;
			break;

                case 9: // KUSV_UNKNOWN
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Q9;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Q9;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Q9;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Q9;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Q9;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Q9;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Q9;
			break;

		default:
			bUpdate = bUpdate && m_cfg_bGnssUpdate_Def;
			m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Def;
			m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Def;
			m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Def;
			m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Def;
			m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Def;
			m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Def;
			break;
		}
	}
	else
	{
		m_dGnssUncertaintyCoef_lat = m_cfg_dGnssUncertaintyCoef_lat_Def;
		m_dGnssUncertaintyCoef_lat_v = m_cfg_dGnssUncertaintyCoef_lat_v_Def;
		m_dGnssUncertaintyCoef_lon = m_cfg_dGnssUncertaintyCoef_lon_Def;
		m_dGnssUncertaintyCoef_lon_v = m_cfg_dGnssUncertaintyCoef_lon_v_Def;
		m_dGnssUncertaintyCoef_h = m_cfg_dGnssUncertaintyCoef_h_Def;
		m_dGnssUncertaintyCoef_h_w = m_cfg_dGnssUncertaintyCoef_h_w_Def;
	}

	// 2-d. GNSS status based update based on the number of satellites
	if (m_cfg_bRunUpdateForGnssNumOfSatLimit == true)
	{
		if (m_MeasGnss.number_of_satellites < (uint32_t)m_cfg_iRunUpdateGnssNumOfSatLimit)	// GNSS satellite in used
		{
			bUpdate = bUpdate && false;
		}
	}
	m_bUpdate = bUpdate;

	// -----------------------------------------------------------------------
	// Execution of Update

	if (bUpdate == true)
	{
		m_nLatestUpdateTimestamp = m_nMeasGnssTimestamp;
		UpdateOneStep();
	}

	return true;
}

//##########################################################################################
// 01. Fault detection & reset
bool CPoseEstimationEKF::RunFaultDetectionBasedOnGnss(int64_t nTimestamp, KUSV_Gnss MeasGnss)
{
	// -----------------------------------------------------------------------
	// Configuration of Fault detection and reset
	if (m_cfg_bOnFaultDetectionBasedOnGnss == false)
	{
		return false;
	}

	// -----------------------------------------------------------------------
	// GNSS validity check
	bool bGnssValidity = true;

	// GNSS validity check using standard deviation
	if (m_cfg_bOnGnssValidityCheckBasedOnStd == true)
	{
		double dGnssMeasPosError = sqrt(
			(double)MeasGnss.latitude_sigma*MeasGnss.latitude_sigma
			+ (double)MeasGnss.longitude_sigma*+MeasGnss.longitude_sigma);
		if (dGnssMeasPosError > m_cfg_dGnssValidityStdThreshold_m)
		{
			bGnssValidity = bGnssValidity && false;
		}
	}

	// GNSS validity check using status
	if (m_cfg_bOnGnssValidityCheckBasedOnQuality == true)
	{
		switch (m_MeasGnss.quality)	// GNSS Quality
		{
                case 0: // KUSV_NO_FIX
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q0;
			break;

                case 1: // KUSV_FIX
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q1;
			break;

                case 2: // KUSV_DIFF_FIX
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q2;
			break;

                case 3: // KUSV_PPS_FIX
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q3;
			break;

                case 4: // KUSV_RTK_INT
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q4;
			break;

                case 5: // KUSV_RTK_FLOAT
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q5;
			break;

                case 6: // KUSV_DR
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q6;
			break;

                case 7: // KUSV_MANUAL
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q7;
			break;

                case 8: // KUSV_SIMULATION
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q8;
			break;

                case 9: // KUSV_UNKNOWN
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Q9;
			break;

		default:
			bGnssValidity = bGnssValidity && m_cfg_bGnssUpdate_Def;
			break;
		}
	}

	// GNSS validity check using status
	if (m_cfg_bOnGnssValidityCheckBasedOnNumOfSat == true)
	{
		if (MeasGnss.number_of_satellites < (uint32_t)m_cfg_iRunUpdateGnssNumOfSatLimit)	// GNSS satellite in used
		{
			bGnssValidity = bGnssValidity && false;
		}
	}


	// -----------------------------------------------------------------------
	// Fault detection by comparing with GNSS
	if (bGnssValidity == true)
	{
		// Distance between geo-position between State and GNSS
		GeoPoint2D GeoPoint_PoseEstimationEKF = { m_matX.at<double>(0, 0), m_matX.at<double>(1, 0) };
		GeoPoint2D GeoPoint_GnssMeasured = { MeasGnss.latitude, MeasGnss.longitude };
		double dDistanceBtwState_Gnss = haversineDistance(&GeoPoint_PoseEstimationEKF, &GeoPoint_GnssMeasured);

		// Heading Distance between geo-position between State and GNSS
		double dHeadingDiffBtwState_Gnss = abs(AngleDiff_deg((double)MeasGnss.heading + 90., m_matX.at<double>(2, 0)));

		// Checking threshold for difference between State and GNSS
		if (dDistanceBtwState_Gnss <= m_cfg_dFaultPositionThreshold_m &&
			dHeadingDiffBtwState_Gnss <= m_cfg_dFaultHeadingThreshold_deg)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	// Debugging information
	PutDebuggingInfo("Fault detection - large distance or heading comparing to Gnss measurement!");

	return true;
}

//##########################################################################################
// 02. prediction
bool CPoseEstimationEKF::PredictionOneStep(void)
{
	// -----------------------------------------------------------------------
	// Calculating sampling time
	int64_t nTimeStampDiff_us = m_nInputMotionTimeStamp_us - m_nTimestampe_us;

	if (nTimeStampDiff_us <= 100) // If no timestamp difference
	{
		// PutDebuggingInfo("No timestamp difference in the prediction!");
		return false;
	}
	// PutDebuggingInfo("Prediction!");

	double dT_SamplingTime = (double)nTimeStampDiff_us / 1000000.; // us -> s 

	// -----------------------------------------------------------------------
	// State (X) propagation using CV model

	// Previous state: Latitude [deg], Longitude [deg], Heading [deg]
	double dPreLat_deg = m_matX.at<double>(0, 0);
	double dPreLon_deg = m_matX.at<double>(1, 0);
	double dPreYaw_deg = m_matX.at<double>(2, 0);
	double dPreYaw_rad = deg2Rad(m_matX.at<double>(2, 0));

	double dLenOfDegLat = lengthOfDegreeLatitude(dPreLat_deg);
	double dLenOfDegLon = lengthOfDegreeLongitude(dPreLat_deg);
	if (abs(dLenOfDegLat) <= 0. || abs(dLenOfDegLon) <= 0.)
	{
		PutDebuggingInfo("Error for calculation of 'Length of degree'!");
		return false;
	}

	// Prediction using CV model
	double dPrevInputYawRate_rad = deg2Rad(m_AUTOMotionPrev.yaw_rate);
	m_matX.at<double>(0, 0) = dPreLat_deg + dT_SamplingTime*m_AUTOMotionPrev.speed_x*sin(dPreYaw_rad) / dLenOfDegLat;
	m_matX.at<double>(1, 0) = dPreLon_deg + dT_SamplingTime*m_AUTOMotionPrev.speed_x*cos(dPreYaw_rad) / dLenOfDegLon;
	m_matX.at<double>(2, 0) = NormAngle_deg(dPreYaw_deg + dT_SamplingTime*m_AUTOMotionPrev.yaw_rate);

	//ROS_INFO("PredictedState : [ %f // %f // %f ]", m_matX.at<double>(0, 0), m_matX.at<double>(1, 0), m_matX.at<double>(2, 0));


	// -----------------------------------------------------------------------
	// Process model matrix (Q) generation
	double dStdStateUncertaintyLatitude_deg =
		(m_cfg_dStateUncertaintyCoef_lat
			+ m_cfg_dStateUncertaintyCoef_lat_v*abs(dT_SamplingTime*m_AUTOMotionPrev.speed_x)
			+ m_cfg_dStateUncertaintyCoef_lat_a*abs(dT_SamplingTime*dT_SamplingTime*m_AUTOMotionPrev.acceleration_x))
		/ dLenOfDegLat;
	double dStdStateUncertaintyLongitude_deg =
		(m_cfg_dStateUncertaintyCoef_lon
			+ m_cfg_dStateUncertaintyCoef_lon_v*abs(dT_SamplingTime*m_AUTOMotionPrev.speed_x)
			+ m_cfg_dStateUncertaintyCoef_lon_a*abs(dT_SamplingTime*dT_SamplingTime*m_AUTOMotionPrev.acceleration_x))
		/ dLenOfDegLon;
	double dStdStateUncertaintyHeading_deg =
		m_cfg_dStateUncertaintyCoef_h
		+ m_cfg_dStateUncertaintyCoef_h_w*abs(dT_SamplingTime*m_AUTOMotionPrev.yaw_rate);

	cv::Mat matQ(STATE_ORDER, STATE_ORDER, CV_64FC1, cv::Scalar(0.));
	matQ.at<double>(0, 0) = dStdStateUncertaintyLatitude_deg*dStdStateUncertaintyLatitude_deg;
	matQ.at<double>(1, 1) = dStdStateUncertaintyLongitude_deg*dStdStateUncertaintyLongitude_deg;
	matQ.at<double>(2, 2) = dStdStateUncertaintyHeading_deg*dStdStateUncertaintyHeading_deg;

	// -----------------------------------------------------------------------
	// Covariance (P) propagation using CV model

	// Compute the Jacobian matrix (F) of CV model
	cv::Mat matF(STATE_ORDER, STATE_ORDER, CV_64FC1, cv::Scalar(0.));
	matF.at<double>(0, 0) = 1.;
	matF.at<double>(0, 1) = 0.;
	matF.at<double>(0, 2) = M_PI / 180. * dT_SamplingTime*m_AUTOMotionPrev.speed_x*cos(dPreYaw_rad) / dLenOfDegLat;
	matF.at<double>(1, 0) = 0.;
	matF.at<double>(1, 1) = 1.;
	matF.at<double>(1, 2) = -M_PI / 180. * dT_SamplingTime*m_AUTOMotionPrev.speed_x*sin(dPreYaw_rad) / dLenOfDegLon;
	matF.at<double>(2, 0) = 0.;
	matF.at<double>(2, 1) = 0.;
	matF.at<double>(2, 2) = 1.;

	// Covariance matrix	
	m_matP = matF * m_matP * matF.t() + matQ;

	return true;
}
void CPoseEstimationEKF::SetMotionInput(int64_t nTimeStamp_us, KUSV_Motion inputMotion)
{
	m_AUTOMotion = inputMotion;
	m_nInputMotionTimeStamp_us = nTimeStamp_us;
}

//##########################################################################################
// 03. Measurement update
bool CPoseEstimationEKF::UpdateOneStep(void)
{
	// Conversion parameter
	double dLenOfDegLat = lengthOfDegreeLatitude(m_matX.at<double>(0, 0));
	double dLenOfDegLon = lengthOfDegreeLongitude(m_matX.at<double>(0, 0));
	if (abs(dLenOfDegLat) <= 0. || abs(dLenOfDegLon) <= 0.)
	{
		PutDebuggingInfo("Error for calculation of 'Length of degree'!");
		return false;
	}

	// Observation matrix: H
	double dataH[MEAS_ORDER][STATE_ORDER] =
	{
		1.,   0.,	  0.,
		0.,   1.,	  0.,
		0.,   0.,	  1.
	};
	cv::Mat matH(MEAS_ORDER, STATE_ORDER, CV_64FC1, dataH);

	// Measurement model: Z_model: 
	double data_z_model[MEAS_ORDER] =
	{
		m_matX.at<double>(0, 0),
		m_matX.at<double>(1, 0),
		m_matX.at<double>(2, 0)
	};
	cv::Mat	matZmodel(MEAS_ORDER, 1, CV_64FC1, data_z_model);

	// Measurement vector: Z
	double data_z[MEAS_ORDER] =
	{
		m_MeasGnss.latitude,
		m_MeasGnss.longitude,
		NormAngle_deg((double)m_MeasGnss.heading + 90.)
	};
	cv::Mat	matZ(MEAS_ORDER, 1, CV_64FC1, data_z);

	// Measurement covariance matrix: R	
	double dStdMeasUncertaintyLatitude_deg =
		((double)m_MeasGnss.latitude_sigma
			+ m_dGnssUncertaintyCoef_lat
			+ m_dGnssUncertaintyCoef_lat_v*abs(m_AUTOMotion.speed_x))
		/ dLenOfDegLat;
	double dStdMeasUncertaintyLongitude_deg =
		((double)m_MeasGnss.longitude_sigma
			+ m_dGnssUncertaintyCoef_lon
			+ m_dGnssUncertaintyCoef_lon_v*abs(m_AUTOMotion.speed_x))
		/ dLenOfDegLon;
	double dStdMeasUncertaintyHeading_deg =
		(double)m_MeasGnss.heading_sigma
		+ m_dGnssUncertaintyCoef_h
		+ m_dGnssUncertaintyCoef_h_w*abs(m_AUTOMotion.yaw_rate);

	double data_R[MEAS_ORDER][MEAS_ORDER] =
	{
		dStdMeasUncertaintyLatitude_deg*dStdMeasUncertaintyLatitude_deg,	0.,				0.,
		0.,				dStdMeasUncertaintyLongitude_deg*dStdMeasUncertaintyLongitude_deg,	0.,
		0.,				0.,					 dStdMeasUncertaintyHeading_deg*dStdMeasUncertaintyHeading_deg
	};
	cv::Mat	matR(MEAS_ORDER, MEAS_ORDER, CV_64FC1, data_R);

	// Observation covariance matrix: S	
	cv::Mat matS = matH*m_matP*matH.t() + matR;

	// Kalman Gain: K
	cv::Mat matK = m_matP * matH.t() * matS.inv();

	// Residual: Y
    cv::Mat	matY = matZ - matZmodel;
	matY.at<double>(2, 0) = AngleDiff_deg(matZmodel.at<double>(2, 0), matZ.at<double>(2, 0));


	// Get the posterior of state & covariance		
	m_matX = m_matX + matK * matY;
	m_matP = m_matP - matK * matH * m_matP;
	m_matX.at<double>(2, 0) = NormAngle_deg(m_matX.at<double>(2, 0));

	return true;
}
bool CPoseEstimationEKF::SetMeasGNSS(int64_t nTimestamp, KUSV_Gnss MeasGnss)
{
	m_MeasGnss = MeasGnss;
	m_nMeasGnssTimestamp = nTimestamp;

	// Fault detection and reset based on Gnss measurement
	if (RunFaultDetectionBasedOnGnss(nTimestamp, MeasGnss))
	{
		// reset the state and weight based on Gnss input
		m_nTimestampe_us = nTimestamp;

		m_matX.at<double>(0, 0) = MeasGnss.latitude;
		m_matX.at<double>(1, 0) = MeasGnss.longitude;
		m_matX.at<double>(2, 0) = NormAngle_deg((double)MeasGnss.heading + 90.);

		m_matP = cv::Scalar(0.);
		double dLenOfDegLat = lengthOfDegreeLatitude(MeasGnss.latitude);
		double dLenOfDegLon = lengthOfDegreeLongitude(MeasGnss.latitude);
		if (abs(dLenOfDegLat) > 0. && abs(dLenOfDegLon) > 0.)
		{
			m_matP.at<double>(0, 0) = pow(m_cfg_dInitStateUncertainty_lat_m / dLenOfDegLat, 2.);
			m_matP.at<double>(1, 1) = pow(m_cfg_dInitStateUncertainty_lon_m / dLenOfDegLon, 2.);
			m_matP.at<double>(2, 2) = pow(m_cfg_dInitStateUncertainty_heading_deg, 2.);
		}

		m_bFlagOfResetForInitPrediction = true;

		return false;
	}

	return true;
}

//##########################################################################################
// 99. Miscellaneous
double CPoseEstimationEKF::NormAngle_deg(double dAngle_deg)
{
	double dAngleBuff_deg = dAngle_deg;

	// Set the input angle into the 0~pi
	while (dAngleBuff_deg > 360.)
		dAngleBuff_deg -= 360.;

	while (dAngleBuff_deg < 0.)
		dAngleBuff_deg += 360.;

	return dAngleBuff_deg;
}
double CPoseEstimationEKF::NormAngle_rad(double dAngle_rad)
{
	double dAngleBuff_rad = dAngle_rad;

	// Set the input angle into the 0~pi
	while (dAngleBuff_rad > M_PI*2.)
		dAngleBuff_rad -= M_PI*2.;

	while (dAngleBuff_rad < 0.)
		dAngleBuff_rad += M_PI*2.;

	return dAngleBuff_rad;
}
double CPoseEstimationEKF::AngleDiff_deg(double dRef_deg, double dRel_deg)
{
	double dAngleDiffBuff_deg = dRel_deg - dRef_deg;

	// calculate angle difference
	if (dAngleDiffBuff_deg > 180.)
	{
		dAngleDiffBuff_deg = dAngleDiffBuff_deg - 360.;
	}
	else if (dAngleDiffBuff_deg < -180.)
	{
		dAngleDiffBuff_deg = dAngleDiffBuff_deg + 360.;
	}

	return dAngleDiffBuff_deg;
}
double CPoseEstimationEKF::AngleDiff_rad(double dRef_rad, double dRel_rad)
{
	double dAngleDiffBuff_rad = dRel_rad - dRef_rad;

	// calculate angle difference
	if (dAngleDiffBuff_rad > M_PI)
	{
		dAngleDiffBuff_rad = dAngleDiffBuff_rad - 2.*M_PI;
	}
	else if (dAngleDiffBuff_rad < -M_PI)
	{
		dAngleDiffBuff_rad = dAngleDiffBuff_rad + 2.*M_PI;
	}

	return dAngleDiffBuff_rad;
}
void CPoseEstimationEKF::PopDebuggingInfo(std::string& strDebuggingInfo)
{
	if (m_cfg_bDebugInfo != true) return;

	if (m_ssDebuggingInfo.str().size() > 0)
	{
		strDebuggingInfo = m_ssDebuggingInfo.str();
		m_ssDebuggingInfo.str("");
	}
	else
	{
		strDebuggingInfo.clear();
	}
}
void CPoseEstimationEKF::PutDebuggingInfo(std::string strDebuggingInfo)
{
	if (m_cfg_bDebugInfo != true) return;

	if (m_ssDebuggingInfo.str().size() < MAX_SIZE_OF_CHARS_FOR_ERR_MSG)
	{
		m_ssDebuggingInfo << strDebuggingInfo << std::endl;
	}
	else
	{
		m_ssDebuggingInfo << "Debugging Buffer Overflow!" << std::endl;
	}
}

//##########################################################################################
// Configuration
bool CPoseEstimationEKF::ParseConfigINIFile()
{
	// 00. [Management]	
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bDebugInfo", m_cfg_bDebugInfo) != true)	return false;


	// Initialization
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitailLatitude_deg", m_cfg_dInitailLatitude_deg) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitailLongitude_deg", m_cfg_dInitailLongitude_deg) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitailHeading_deg", m_cfg_dInitailHeading_deg) != true)	return false;

	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitStateUncertainty_lat_m", m_cfg_dInitStateUncertainty_lat_m) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitStateUncertainty_lon_m", m_cfg_dInitStateUncertainty_lon_m) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dInitStateUncertainty_heading_deg", m_cfg_dInitStateUncertainty_heading_deg) != true)	return false;



	// EKF Prediction decision management
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bRunPredictionForSpeedLimit", m_cfg_bRunPredictionForSpeedLimit) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dRunPredictionSpeedLimit_kph", m_cfg_dRunPredictionSpeedLimit_kph) != true)	return false;

	// EKF update decision management
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bRunUpdateForTimestampVariation", m_cfg_bRunUpdateForTimestampVariation) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bRunUpdateForSpeedLimit", m_cfg_bRunUpdateForSpeedLimit) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_dRunUpdateSpeedLimit_kph", m_cfg_dRunUpdateSpeedLimit_kph) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bRunUpdateForGnssQuality", m_cfg_bRunUpdateForGnssQuality) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_bRunUpdateForGnssNumOfSatLimit", m_cfg_bRunUpdateForGnssNumOfSatLimit) != true)	return false;
	if(m_ini_parser.ParseConfig("Management", "m_cfg_iRunUpdateGnssNumOfSatLimit", m_cfg_iRunUpdateGnssNumOfSatLimit) != true)	return false;

	// 01. [FaultDetection]	

	// Fault detection and reset
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_bOnFaultDetectionBasedOnGnss", m_cfg_bOnFaultDetectionBasedOnGnss) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_bOnGnssValidityCheckBasedOnStd", m_cfg_bOnGnssValidityCheckBasedOnStd) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_dGnssValidityStdThreshold_m", m_cfg_dGnssValidityStdThreshold_m) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_bOnGnssValidityCheckBasedOnQuality", m_cfg_bOnGnssValidityCheckBasedOnQuality) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_bOnGnssValidityCheckBasedOnNumOfSat", m_cfg_bOnGnssValidityCheckBasedOnNumOfSat) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_dFaultPositionThreshold_m", m_cfg_dFaultPositionThreshold_m) != true)	return false;
	if(m_ini_parser.ParseConfig("FaultDetection", "m_cfg_dFaultHeadingThreshold_deg", m_cfg_dFaultHeadingThreshold_deg) != true)	return false;

	// 02. [Prediction]

	// State uncertainty
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lat", m_cfg_dStateUncertaintyCoef_lat) != true)	return false;
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lat_v", m_cfg_dStateUncertaintyCoef_lat_v) != true)	return false;
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lat_a", m_cfg_dStateUncertaintyCoef_lat_a) != true)	return false;

	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lon", m_cfg_dStateUncertaintyCoef_lon) != true)	return false;
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lon_v", m_cfg_dStateUncertaintyCoef_lon_v) != true)	return false;
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_lon_a", m_cfg_dStateUncertaintyCoef_lon_a) != true)	return false;

	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_h", m_cfg_dStateUncertaintyCoef_h) != true)	return false;
	if(m_ini_parser.ParseConfig("Prediction", "m_cfg_dStateUncertaintyCoef_h_w", m_cfg_dStateUncertaintyCoef_h_w) != true)	return false;


	// 03. [MeasUpdate]
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q0", m_cfg_bGnssUpdate_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q0", m_cfg_dGnssUncertaintyCoef_lat_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q0", m_cfg_dGnssUncertaintyCoef_lat_v_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q0", m_cfg_dGnssUncertaintyCoef_lon_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q0", m_cfg_dGnssUncertaintyCoef_lon_v_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q0", m_cfg_dGnssUncertaintyCoef_h_Q0) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q0", m_cfg_dGnssUncertaintyCoef_h_w_Q0) != true)	return false;
	
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q1", m_cfg_bGnssUpdate_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q1", m_cfg_dGnssUncertaintyCoef_lat_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q1", m_cfg_dGnssUncertaintyCoef_lat_v_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q1", m_cfg_dGnssUncertaintyCoef_lon_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q1", m_cfg_dGnssUncertaintyCoef_lon_v_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q1", m_cfg_dGnssUncertaintyCoef_h_Q1) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q1", m_cfg_dGnssUncertaintyCoef_h_w_Q1) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q2", m_cfg_bGnssUpdate_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q2", m_cfg_dGnssUncertaintyCoef_lat_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q2", m_cfg_dGnssUncertaintyCoef_lat_v_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q2", m_cfg_dGnssUncertaintyCoef_lon_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q2", m_cfg_dGnssUncertaintyCoef_lon_v_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q2", m_cfg_dGnssUncertaintyCoef_h_Q2) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q2", m_cfg_dGnssUncertaintyCoef_h_w_Q2) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q3", m_cfg_bGnssUpdate_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q3", m_cfg_dGnssUncertaintyCoef_lat_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q3", m_cfg_dGnssUncertaintyCoef_lat_v_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q3", m_cfg_dGnssUncertaintyCoef_lon_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q3", m_cfg_dGnssUncertaintyCoef_lon_v_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q3", m_cfg_dGnssUncertaintyCoef_h_Q3) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q3", m_cfg_dGnssUncertaintyCoef_h_w_Q3) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q4", m_cfg_bGnssUpdate_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q4", m_cfg_dGnssUncertaintyCoef_lat_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q4", m_cfg_dGnssUncertaintyCoef_lat_v_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q4", m_cfg_dGnssUncertaintyCoef_lon_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q4", m_cfg_dGnssUncertaintyCoef_lon_v_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q4", m_cfg_dGnssUncertaintyCoef_h_Q4) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q4", m_cfg_dGnssUncertaintyCoef_h_w_Q4) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q5", m_cfg_bGnssUpdate_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q5", m_cfg_dGnssUncertaintyCoef_lat_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q5", m_cfg_dGnssUncertaintyCoef_lat_v_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q5", m_cfg_dGnssUncertaintyCoef_lon_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q5", m_cfg_dGnssUncertaintyCoef_lon_v_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q5", m_cfg_dGnssUncertaintyCoef_h_Q5) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q5", m_cfg_dGnssUncertaintyCoef_h_w_Q5) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q6", m_cfg_bGnssUpdate_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q6", m_cfg_dGnssUncertaintyCoef_lat_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q6", m_cfg_dGnssUncertaintyCoef_lat_v_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q6", m_cfg_dGnssUncertaintyCoef_lon_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q6", m_cfg_dGnssUncertaintyCoef_lon_v_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q6", m_cfg_dGnssUncertaintyCoef_h_Q6) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q6", m_cfg_dGnssUncertaintyCoef_h_w_Q6) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q7", m_cfg_bGnssUpdate_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q7", m_cfg_dGnssUncertaintyCoef_lat_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q7", m_cfg_dGnssUncertaintyCoef_lat_v_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q7", m_cfg_dGnssUncertaintyCoef_lon_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q7", m_cfg_dGnssUncertaintyCoef_lon_v_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q7", m_cfg_dGnssUncertaintyCoef_h_Q7) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q7", m_cfg_dGnssUncertaintyCoef_h_w_Q7) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q8", m_cfg_bGnssUpdate_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q8", m_cfg_dGnssUncertaintyCoef_lat_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q8", m_cfg_dGnssUncertaintyCoef_lat_v_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q8", m_cfg_dGnssUncertaintyCoef_lon_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q8", m_cfg_dGnssUncertaintyCoef_lon_v_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q8", m_cfg_dGnssUncertaintyCoef_h_Q8) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q8", m_cfg_dGnssUncertaintyCoef_h_w_Q8) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Q9", m_cfg_bGnssUpdate_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Q9", m_cfg_dGnssUncertaintyCoef_lat_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Q9", m_cfg_dGnssUncertaintyCoef_lat_v_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Q9", m_cfg_dGnssUncertaintyCoef_lon_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Q9", m_cfg_dGnssUncertaintyCoef_lon_v_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Q9", m_cfg_dGnssUncertaintyCoef_h_Q9) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Q9", m_cfg_dGnssUncertaintyCoef_h_w_Q9) != true)	return false;

	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_bGnssUpdate_Def", m_cfg_bGnssUpdate_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_Def", m_cfg_dGnssUncertaintyCoef_lat_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lat_v_Def", m_cfg_dGnssUncertaintyCoef_lat_v_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_Def", m_cfg_dGnssUncertaintyCoef_lon_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_lon_v_Def", m_cfg_dGnssUncertaintyCoef_lon_v_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_Def", m_cfg_dGnssUncertaintyCoef_h_Def) != true)	return false;
	if(m_ini_parser.ParseConfig("MeasUpdate", "m_cfg_dGnssUncertaintyCoef_h_w_Def", m_cfg_dGnssUncertaintyCoef_h_w_Def) != true)	return false;

	// 99. [Miscellaneous]



	return true;
}
// bool CPoseEstimationEKF::SaveEKFStateCovToIniConfig(void)
// {
// 	if (m_strCfgINIFilePath.length() <= 0) {
// 		return false;
// 	}

// 	bool bIsSuccess = true; // Return type (save success)

// 	std::stringstream strStreamKeyName;
// 	std::stringstream strStreamValue;

// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitailLatitude_deg";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetState(0);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}
// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitailLongitude_deg";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetState(1);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}
// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitailHeading_deg";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetState(2);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}

// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitStateUncertainty_lat_m";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetCovariance(0, 0);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}
// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitStateUncertainty_lon_m";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetCovariance(1, 1);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}
// 	strStreamKeyName.str(""); strStreamKeyName.clear();
// 	strStreamKeyName << "m_cfg_dInitStateUncertainty_heading_deg";
// 	strStreamValue.str(""); strStreamValue.clear();
// 	strStreamValue << GetCovariance(2, 2);
// 	if (WritePrivateProfileString("Management", strStreamKeyName.str().c_str(), strStreamValue.str().c_str(), m_strCfgINIFilePath.c_str()) <= 0) {
// 		bIsSuccess = false;
// 	}

// 	return bIsSuccess;
// }
