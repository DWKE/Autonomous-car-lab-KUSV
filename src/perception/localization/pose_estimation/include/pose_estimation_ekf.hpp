/**
 * @copyright (c) Valeo Driving Assistance Research.
 * <br>All rights reserved. Subject to limited distribution and restricted disclosure only.
 * @author kichun.jo@valeo.com
 * @file pose_ekf.hpp
 * @brief Pose estimation EKF header file
 * @version 1.0
 * @date 15-09-2016
 * @bug	No known bugs
 * @warning	No warnings
 */

#pragma once

 /*---------------------------------------------------------------------------
 ** Includes
 */
#include <string>
#include <sstream>
#include <iomanip>

#include "kusv_pose_interface.h" //Modify Path!!!
#include "ini_parser.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

 /*---------------------------------------------------------------------------
 ** Defines and Macros
 */

 /*---------------------------------------------------------------------------
 ** Typedefs and Functions
 */

 /**
  * @class CPoseEstimationEKF
  * @brief Class of component for pose estimation
  * @descriptions state = [lat_deg, lon_deg, heading_deg]
  */

class CPoseEstimationEKF
{

	//##########################################################################################
	// 00. Pose Estimation Management

	// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $
	//	a. Initialization
private:

public:
	CPoseEstimationEKF();
	~CPoseEstimationEKF();
	bool    Init(int64_t nTimestamp, std::string strCfgINIfilePath);

private:
	bool	m_bFlagInitComplete;


	// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $
	//	b. Estimated state and covariance definition
private:
	// filter state
	const static int STATE_ORDER = 3;	// State = [latitude, longitude, heading]
	const static int MEAS_ORDER = 3;	// Measurement = [latitude, longitude, heading]

public:
	cv::Mat			GetState(void) { return m_matX; }
	double			GetState(int nRow);
	cv::Mat			GetCovariance(void) { return m_matP; }
	double			GetCovariance(int nRow, int nCol);
        KUSV_Gnss		GetEstimatedGnss(void);
        KUSV_Gnss		GetEstimatedGnss(double dEastOffset_m, double dNorthOffset_m);
	int64_t			GetTimestampe(void) { return m_nTimestampe_us; }
		
private:
	cv::Mat			m_matX;	// state
	cv::Mat			m_matP;	// covariance
	int64_t			m_nTimestampe_us;
	
	// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $
	//	c. Update management
public:
	bool	RunPrediction(void);
	bool	RunUpdate(void);

private:
	bool	m_bPrediction;
	bool	m_bUpdate;

	//##########################################################################################
	// 01. Fault detection
public:
        bool RunFaultDetectionBasedOnGnss(int64_t nTimestamp, KUSV_Gnss MeasGnss);
	

	//##########################################################################################
	// 02. prediction
public:
	bool	PredictionOneStep(void);
		
public:
        void	SetMotionInput(int64_t nTimeStamp_us, KUSV_Motion inputMotion);

	//	Model Input
private:
        KUSV_Motion m_AUTOMotion;
	int64_t m_nInputMotionTimeStamp_us;

        KUSV_Motion m_AUTOMotionPrev;
	bool	m_bFlagOfResetForInitPrediction; // Init & reset flag	
	



	//##########################################################################################
	// 03. Measurement update
public:
	bool	UpdateOneStep(void);

private:
	int64_t	m_nLatestUpdateTimestamp;

public:
        bool	SetMeasGNSS(int64_t nTimestamp, KUSV_Gnss MeasGnss);

private:
        KUSV_Gnss m_MeasGnss;
	int64_t	m_nMeasGnssTimestamp;

	double	m_dGnssUncertaintyCoef_lat;
	double	m_dGnssUncertaintyCoef_lat_v;
	double	m_dGnssUncertaintyCoef_lon;
	double	m_dGnssUncertaintyCoef_lon_v;
	double	m_dGnssUncertaintyCoef_h;
	double	m_dGnssUncertaintyCoef_h_w;


	//##########################################################################################
	// 99. Miscellaneous
	

	// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $
	//	a. Normalization to ENU coordinate angle (0-360 deg)
public:
	double NormAngle_deg(double dAngle_deg);
	double NormAngle_rad(double dAngle_rad);
	double AngleDiff_deg(double dRef_deg, double dRel_deg);
	double AngleDiff_rad(double dRef_rad, double dRel_rad);


	// $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $ $
	//	b. Debugging status

public:
	void PopDebuggingInfo(std::string& strDebuggingInfo);

private:
	const static int MAX_SIZE_OF_CHARS_FOR_ERR_MSG = 500;
	void	PutDebuggingInfo(std::string strDebuggingInfo);
	std::stringstream	m_ssDebuggingInfo;
	
	//##########################################################################################
	// Configuration

public:
	bool ParseConfigINIFile();

private:
	const static int FILE_PATH_BUFF_SIZE = 255;
	CINI_Parser m_ini_parser;


	// 00. [Management]
private:
	bool	m_cfg_bDebugInfo;		// Debug info

public:
	int		GetCfgNumOfState(void) { return STATE_ORDER; }
	bool	GetCfgDebugInfo(void) { return m_cfg_bDebugInfo; }

	// Initialization
private:
	double	m_cfg_dInitailLatitude_deg;
	double	m_cfg_dInitailLongitude_deg;
	double	m_cfg_dInitailHeading_deg;

	double	m_cfg_dInitStateUncertainty_lat_m;
	double	m_cfg_dInitStateUncertainty_lon_m;
	double	m_cfg_dInitStateUncertainty_heading_deg;
	
public:
	// bool	SaveEKFStateCovToIniConfig(void);
	
private:
	// EKF Prediction decision management
	bool	m_cfg_bRunPredictionForSpeedLimit;
	double	m_cfg_dRunPredictionSpeedLimit_kph;

	// EKF update decision management
	bool	m_cfg_bRunUpdateForTimestampVariation;

	bool	m_cfg_bRunUpdateForSpeedLimit;
	double	m_cfg_dRunUpdateSpeedLimit_kph;

	bool	m_cfg_bRunUpdateForGnssQuality;

	bool	m_cfg_bRunUpdateForGnssNumOfSatLimit;
	int		m_cfg_iRunUpdateGnssNumOfSatLimit;

	// 01. [FaultDetection]		
private:
	bool	m_cfg_bOnFaultDetectionBasedOnGnss;

	bool	m_cfg_bOnGnssValidityCheckBasedOnStd;
	double	m_cfg_dGnssValidityStdThreshold_m;

	bool	m_cfg_bOnGnssValidityCheckBasedOnQuality;

	bool	m_cfg_bOnGnssValidityCheckBasedOnNumOfSat;

	double	m_cfg_dFaultPositionThreshold_m;
	double	m_cfg_dFaultHeadingThreshold_deg;
	

	// 02. [Prediction]		

	// State uncertainty
private:
	double	m_cfg_dStateUncertaintyCoef_lat;
	double	m_cfg_dStateUncertaintyCoef_lat_v;
	double	m_cfg_dStateUncertaintyCoef_lat_a;

	double	m_cfg_dStateUncertaintyCoef_lon;
	double	m_cfg_dStateUncertaintyCoef_lon_v;
	double	m_cfg_dStateUncertaintyCoef_lon_a;

	double	m_cfg_dStateUncertaintyCoef_h;
	double	m_cfg_dStateUncertaintyCoef_h_w;
	

	// 03. [MeasUpdate]

	// Artificial uncertainty (_xx means quality)
private:

	// Quality 0: Fix not available
	bool	m_cfg_bGnssUpdate_Q0;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q0;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q0;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q0;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q0;
	double	m_cfg_dGnssUncertaintyCoef_h_Q0;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q0;

	// Quality 1: GNSS fix
	bool	m_cfg_bGnssUpdate_Q1;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q1;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q1;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q1;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q1;
	double	m_cfg_dGnssUncertaintyCoef_h_Q1;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q1;

	// Quality 2: Differential GPS fix (values above 2 are 2.3 features)
	bool	m_cfg_bGnssUpdate_Q2;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q2;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q2;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q2;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q2;
	double	m_cfg_dGnssUncertaintyCoef_h_Q2;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q2;

	// Quality 3: PPS fix
	bool	m_cfg_bGnssUpdate_Q3;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q3;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q3;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q3;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q3;
	double	m_cfg_dGnssUncertaintyCoef_h_Q3;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q3;

	// Quality 4: Real Time Kinematic
	bool	m_cfg_bGnssUpdate_Q4;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q4;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q4;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q4;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q4;
	double	m_cfg_dGnssUncertaintyCoef_h_Q4;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q4;

	// Quality 5: Float RTK
	bool	m_cfg_bGnssUpdate_Q5;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q5;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q5;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q5;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q5;
	double	m_cfg_dGnssUncertaintyCoef_h_Q5;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q5;

	// Quality 6: Estimated (dead reckoning) 
	bool	m_cfg_bGnssUpdate_Q6;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q6;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q6;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q6;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q6;
	double	m_cfg_dGnssUncertaintyCoef_h_Q6;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q6;

	// Quality 7: Manual input mode
	bool	m_cfg_bGnssUpdate_Q7;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q7;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q7;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q7;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q7;
	double	m_cfg_dGnssUncertaintyCoef_h_Q7;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q7;

	// Quality 8: Simulation mode
	bool	m_cfg_bGnssUpdate_Q8;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q8;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q8;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q8;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q8;
	double	m_cfg_dGnssUncertaintyCoef_h_Q8;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q8;

	// Quality 9: Unknown
	bool	m_cfg_bGnssUpdate_Q9;
	double	m_cfg_dGnssUncertaintyCoef_lat_Q9;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Q9;
	double	m_cfg_dGnssUncertaintyCoef_lon_Q9;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Q9;
	double	m_cfg_dGnssUncertaintyCoef_h_Q9;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Q9;

	// Quality Default
	bool	m_cfg_bGnssUpdate_Def;
	double	m_cfg_dGnssUncertaintyCoef_lat_Def;
	double	m_cfg_dGnssUncertaintyCoef_lat_v_Def;
	double	m_cfg_dGnssUncertaintyCoef_lon_Def;
	double	m_cfg_dGnssUncertaintyCoef_lon_v_Def;
	double	m_cfg_dGnssUncertaintyCoef_h_Def;
	double	m_cfg_dGnssUncertaintyCoef_h_w_Def;

};


/*---------------------------------------------------------------------------
** End of File
*/
