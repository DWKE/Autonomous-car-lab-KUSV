/**
  @file  		CAN_Tx.cpp
  @brief 		CAN TX

  @author  		elfmawang (kjh)
  @E-mail		kjh23135 (at) gmail.com
  @internal
  |+--Created		18-07-14
  |+--Compiler      ROS
  |+--Company		Konkuk Univ. Control Research Lab
  |+--Copyright  	Copyright (c) 2018, elfmawang

  @Revision
  =====================================================================
  */

#include <ros/ros.h>
#include <can_msgs/Frame.h>
#include "local_planning/Keyboard_Interface.h"
#include "kusv_msgs/kusv_CanInfo.h"
#include "local_planning/kusv_Control_CanInfo.h"
#include "local_planning/kusv_ControlCmd.h"
#include "kusv_msgs/kusv_GlobalPose.h"
#include <string>
#include <arpa/inet.h>
#include <math.h>

#define CAN_ID_MO_CONF		0x156
#define CAN_ID_MO_VAL		0x157
#define CAN_ID_REPORT_EPS	0x710
#define CAN_ID_REPORT_ACC	0x711

#define CAN_ID_GWAY1		0x100
#define CAN_ID_GWAY2		0x101
#define CAN_ID_GWAY3		0x102
#define CAN_ID_GWAY4		0x103
#define CAN_ID_GWAY5		0x123

#define CAN_ID_MsgLat		0x110
#define CAN_ID_MsgLon		0x111
#define CAN_ID_MsgYaw		0x112
#define CAN_ID_MsgState		0x113

//#define CAN_ID_TEST			0x110


#define Disable		0
#define Enable		1
#define Override	0
#define Ignore		1


enum MODE_KEYBOARD
{
	NONE			= 0
	, BOTH_EN		= 1
	, EPS_EN		= 2 
	, ACC_EN 		= 3
	, CONTROL 		= 4
	, SPEEDUP		= 5
	, SPEEDDOWN		= 6
	, STRLEFT		= 7
	, STRRIGHT		= 8
	, QUIT_CTRL		= 9
};

// Structure Define
#pragma pack(1)
//----------- Control Can
typedef union _CAN_MSG_MO_CONF_
{
	uint8_t CAN_MO_CONF_Data[8];
	struct{
		//0
		uint8_t EPS_En	 	: 1;
		uint8_t reserveb1 	: 1;
		uint8_t EPS_IGNORE 	: 1;
		uint8_t reserveb2 	: 5;
		//1
		uint8_t EPS_Slvel;
		//2
		uint8_t ACC_En 			: 1;
		uint8_t reserveb3 		: 3;
		uint8_t ACC_StopRequest : 1;
		uint8_t reserveb4 		: 1;
		uint8_t ACC_aeb_seq 	: 1;
		uint8_t reserveb5 		: 1;
		//3
		uint8_t SetDispSpeed;
		//4
		uint8_t reserveb6;
		uint8_t reserveb7;
		uint8_t reserveb8;
		// 7
		uint8_t Mo_AlvCnt;
	}Str;
}CAN_MSG_MO_CONF;

typedef union _CAN_MSG_MO_VAL_
{
	uint8_t CAN_MO_VAL_Data[8];
	struct{
		int16_t Steer_Cmd	:16; 
		uint8_t reserveb_1	:8;
		uint16_t Accel_Dec_Cmd	:16;
		uint8_t Aeb_DecCmd	:8;
		uint16_t reserveb2	:16; 
	}Str;
}CAN_MSG_MO_VAL;

typedef union _CAN_MSG_REPORT_EPS_
{
	uint8_t CAN_REPORT_EPS_Data[8];
	struct{
		//0
		uint8_t EPS_Fd_En 			: 1;
		uint8_t Mo_Fd_State 		: 3;
		uint8_t EPS_Fd_ErrBit_PD 	: 1;
		uint8_t EPS_Fd_ErrBit_EPS	: 1;
		uint8_t EPS_Fd_ErrBit_Vinfo : 1;
		uint8_t EPS_Fd_ErrBit_SAS	: 1;
		//1
		uint8_t Mo_Fd_Seq			: 4;
		uint8_t EPS_Fd_Ovrrd_Ignore	: 1;
		uint8_t EPS_Fd_Override		: 1;
		uint8_t reserveb1			: 2;
		//2
		int16_t EPS_Fd_SteerAngle;
		int16_t EPS_Fd_Steer_Torq	: 12;
		int16_t EPS_Fd_Str_out_tq	: 12;
		uint8_t Mo_Fd_AlvCnt;

	}Str;
}CAN_MSG_REPORT_EPS;

typedef union _CAN_MSG_REPORT_ACC_
{
	uint8_t CAN_REPORT_ACC_Data[8];
	struct{
		//0
		uint8_t ACC_Fd_En 			: 1;
		uint8_t Mo_Fd_State			: 3;
		uint8_t ACC_Fd_ErrBit_PD	: 1;
		uint8_t reserveb1			: 1;
		uint8_t ACC_Fd_ErrBit_TCU	: 1;
		uint8_t ACC_Fd_ErrBit_ACC	: 1;
		//1
		uint8_t Mo_Fd_Seq			: 4;
		uint8_t reserveb2			: 2;
		uint8_t ACC_Fd_aeb_seq		: 1;
		uint8_t reserveb3			: 1;
		//2
		uint8_t ACC_Fd_VSpeed;
		uint8_t ACC_Fd_SetSpeed;
		int16_t LONG_ACCEL			: 11;
		uint8_t reserveb4			: 5;
		uint8_t ACC_Fd_Ovrrd_Rturn_sig : 1;
		uint8_t ACC_Fd_Ovrrd_Haz_sig : 1;
		uint8_t ACC_Fd_Ovrrd_Lturn_sig : 1;
		uint8_t reserveb5			: 1;
		uint8_t G_SEL_DISP			: 4;
		uint8_t Mo_Fd_AlvCnt;
	}Str;
}CAN_MSG_REPORT_ACC;

//-------------- CAN GW
typedef union _CAN_MSG_GWAY1_
{
	uint8_t CAN_GWAY1_Data[8];
	struct{
		uint8_t Gway_Wheel_Velocity_FR_L;
		uint8_t Gway_Wheel_Velocity_FR_H	: 6;
		uint8_t reserve1			: 2;
		uint8_t Gway_Wheel_Velocity_RL_L;
		uint8_t Gway_Wheel_Velocity_RL_H	: 6;	
		uint8_t reserve2			: 2;
		uint8_t Gway_Wheel_Velocity_RR_L;
		uint8_t Gway_Wheel_Velocity_RR_H	: 6;
		uint8_t reserve3			: 2;
		uint8_t Gway_Wheel_Velocity_FL_L;
		uint8_t Gway_Wheel_Velocity_FL_H	: 6;
		uint8_t reserve4			: 2;

	}Str;
}CAN_MSG_GWAY1;

typedef union _CAN_MSG_GWAY2_
{
	uint8_t CAN_GWAY2_Data[8];
	struct{
		uint8_t Gway_Lateral_Accel_Speed_L;
		uint8_t Gway_Lateral_Accel_Speed_H;
		uint8_t Gway_Parking_Brake_Active	: 4;
		uint8_t Gway_AirConditioner_On		: 4;
		uint8_t Gway_Steering_Angle_L;
		uint8_t Gway_Steering_Angle_H;
		uint8_t Gway_Steering_Speed;
		uint8_t Gway_Steering_Tq_L;
		uint8_t Gway_Steering_Tq_H;
	}Str;
}CAN_MSG_GWAY2;

typedef union _CAN_MSG_GWAY3_
{
	uint8_t CAN_GWAY3_Data[8];
	struct{
		uint8_t Gway_Accel_Pedal_Position;
		uint8_t Gway_Brake_Active			: 4;
		uint8_t Gway_BrakeMasterCylinder_Pressure_L	: 4;
		uint8_t Gway_BrakeMasterCylinder_Pressure_M;
		uint8_t Gway_BrakeMasterCylinder_Pressure_H	: 4;
		uint8_t Gway_Engine_Speed_L	:4;
		uint8_t Gway_Engine_Speed_M;
		uint8_t Gway_Engine_Speed_H	:4;
		uint8_t Gway_Gear_Target_Change	: 4;
		uint8_t Gway_GearSelDisp			: 4;
		uint8_t Gway_Throttle_Position_L		: 4;
		uint8_t Gway_Throttle_Position_H		: 4;
		uint8_t Gway_Reserved1				: 4;
	}Str;
}CAN_MSG_GWAY3;

typedef union _CAN_MSG_GWAY4_
{
	uint8_t CAN_GWAY4_Data[8];
	struct{
		uint8_t Gway_Cluster_Odometer_L;
		uint8_t Gway_Cluster_Odometer_M;	
		uint8_t Gway_Cluster_Odometer_H;
		uint8_t Gway_Longitudinal_Accel_Speed_L;
		uint8_t Gway_Longitudinal_Accel_Speed_H;
		uint8_t Gway_Vehicle_Speed_Engine;
		uint8_t Gway_Yaw_Rate_Sensor_L;
		uint8_t Gway_Yaw_Rate_Sensor_H;
	}Str;
}CAN_MSG_GWAY4;

typedef union _CAN_MSG_GWAY5_
{
	uint8_t CAN_GWAY5_Data[8];
	struct{
		uint8_t Dummy0;
		uint8_t Dummy1;
		uint8_t Dummy2;
		uint8_t Dummy3;
		uint8_t Dummy4;
		uint8_t Dummy5;
		uint8_t Dummy6;
		uint8_t Dummy7;
	}Str;
}CAN_MSG_GWAY5;


// Report EPS Message Variables
typedef struct Report_EPS_T_
{
	uint8_t EPS_Fd_En;
	uint8_t Mo_Fd_State;
	uint8_t Mo_Fd_Seq;
	uint8_t EPS_Fd_ErrBit_PD;
	uint8_t EPS_Fd_ErrBit_EPS;
	uint8_t EPS_Fd_ErrBit_Vinfo;
	uint8_t EPS_Fd_ErrBit_SAS;
	uint8_t	EPS_Fd_Ovrrd_Ignore;
	uint8_t EPS_Fd_Override;
	float 	EPS_Fd_SteerAngle;
	float 	EPS_Fd_Steer_Torq;
	float	EPS_Fd_Str_out_tq;
	uint8_t	Mo_Fd_AlvCnt;
}Report_EPS_T;
Report_EPS_T RPT_EPS_T;
// Report ACC Message Variables
typedef struct Report_ACC_T_
{
	uint8_t	ACC_Fd_En;
	uint8_t	Mo_Fd_State;
	uint8_t Mo_Fd_Seq;
	uint8_t ACC_Fd_ErrBit_PD;
	uint8_t	ACC_Fd_ErrBit_TCU;
	uint8_t	ACC_Fd_ErrBit_ACC;
	uint8_t	ACC_Fd_aeb_seq;
	uint8_t	ACC_Fd_VSpeed;
	uint8_t	ACC_Fd_SetSpeed;
	float	LONG_ACCEL;
	uint8_t	ACC_Fd_Ovrrd_Rturn_sig;
	uint8_t	ACC_Fd_Ovrrd_Haz_sig;
	uint8_t	ACC_Fd_Ovrrd_Lturn_sig;
	uint8_t	G_SEL_DISP;
	uint8_t	Mo_Fd_AlvCnt;
}Report_ACC_T;
Report_ACC_T RPT_ACC_T;



#pragma pack(push, 4)

CAN_MSG_MO_CONF mo_conf;
CAN_MSG_MO_VAL mo_val;
CAN_MSG_REPORT_EPS rpt_eps;
CAN_MSG_REPORT_ACC rpt_acc;
CAN_MSG_GWAY1 gway1;
CAN_MSG_GWAY2 gway2;
CAN_MSG_GWAY3 gway3;
CAN_MSG_GWAY4 gway4;
CAN_MSG_GWAY5 gway5;





// publish
uint8_t data_msg[8];
can_msgs::Frame msg;
kusv_msgs::kusv_CanInfo msg_CAN;
local_planning::kusv_Control_CanInfo rpt_CCAN_msg;
kusv_msgs::kusv_GlobalPose gps_msg;

// Control
double Acc_value = -3;
double Angle = 0;


// Wheel speed
float FL, FR, RL, RR;
float Lat_Acc_Speed;
float StAngle;
float St_Tq;
float brake_press;
float engine_speed;
uint8_t throttle_po;
float acc_pedal_pos;
uint8_t brake_active;
uint8_t Gear_target_change;
uint8_t GearSelDisp;


uint8_t Parking_brake_active;
uint8_t Air_cond_on;
uint16_t St_angular;

float Yaw_rate;
uint8_t Vspeed;
float Lon_Acc_Speed;
float Odometer;

// Speed control PID
float Gain_P = 0.09;
float Speed_Error = 0;
float Dest_Speed = 0;
float PID_speed = 0;
float f_Vspeed = 0;

// Message about Jo Ki Chun
double msgLatitude = 0;
double msgLongitude = 0;
float msgHeading = 0;
uint16_t GPSQuality = 0;
uint16_t LatStd = 0;
uint16_t LonStd = 0;
uint16_t YawStd = 0;

// Test CAN Message
uint8_t CAN_Message_1[8]= {0x11, 0x22, 0x33, 0x55, 0x88, 0x99, 0xAA, 0xCC};

// function
void canmsgCallback(const can_msgs::Frame::ConstPtr& Lmsg);
void canmsg2Callback(const can_msgs::Frame::ConstPtr& L2msg);
void KBCallback(const local_planning::Keyboard_Interface::ConstPtr& msg);
void kusvcmdCallback(const local_planning::kusv_ControlCmd::ConstPtr& cmdmsg);
uint8_t CAN_TX_Publish(ros::NodeHandle n, ros::Publisher pub, uint16_t id, uint8_t dlc, uint8_t* data);
uint8_t CAN_Info_Publish(ros::NodeHandle n, ros::Publisher pub);
void Print_for_Test(void);
void Speed_Control_PID(void);
uint8_t CAN_Control_report_Publish(ros::NodeHandle n, ros::Publisher pub);

int main(int argc, char** argv)
{
	ros::init(argc, argv, "CAN_Tx_Test");
	ros::NodeHandle nh;
	ros::NodeHandle nh2;
	ros::NodeHandle nn;
	ros::NodeHandle nn2;
	ros::Publisher aggm_pub = nh.advertise<can_msgs::Frame>("can_rx", 500);		// can Tx setting
	ros::Subscriber aggm_sub = nn.subscribe("can_tx",500, canmsgCallback);		// can Rx setting 
	ros::Subscriber aggm_sub2 = nn.subscribe("can_tx2",500, canmsg2Callback);		// can Rx setting 
	ros::Subscriber aggm_kb_sub = nn2.subscribe("KBIF_msg", 100, KBCallback);	// Keyboard_Interface setting
        ros::Publisher aggm_pub4 = nh2.advertise<kusv_msgs::kusv_CanInfo>("kusv_CanInfo",100);
        ros::Publisher aggm_CCAN_pub = nh.advertise<local_planning::kusv_Control_CanInfo>("CCAN_Info",200);
        ros::Publisher aggm_pub5 = nh.advertise<kusv_msgs::kusv_GlobalPose>("msgPOSE_kichun", 500);

        ros::Subscriber kusv_cmd_sub = nn2.subscribe("control_cmd",500, kusvcmdCallback);

	ros::AsyncSpinner spinner(4);				// use 3 threads 1. can tx, 2. can rx, 3. Keyboard_Interface  // can rx2
	
	ros::Rate loop_rate(50);		//Looping 50Hz
	spinner.start();

	// value initialization
	mo_val.Str.Accel_Dec_Cmd = (uint16_t)((Acc_value + 10.23) * 100);		// -3m^2 
	mo_val.Str.Steer_Cmd = 0;
	mo_conf.Str.EPS_Slvel = 250;
	mo_conf.Str.EPS_IGNORE = 0;			// EPS Override mode
        mo_conf.Str.ACC_aeb_seq = 0;		// AEB Enable
        mo_val.Str.Aeb_DecCmd = 0;		// value / 100

	while(ros::ok())
	{
		//ROS_INFO("Test Message");
		f_Vspeed = (RL + RR)/2;
		mo_conf.Str.SetDispSpeed = f_Vspeed;
		mo_val.Str.Steer_Cmd = Angle * 10;
		Speed_Control_PID();
		Print_for_Test();
		mo_val.Str.Accel_Dec_Cmd = (uint16_t)((PID_speed + 10.23) * 100);

		mo_conf.Str.Mo_AlvCnt++;
		CAN_TX_Publish(nh, aggm_pub, CAN_ID_MO_CONF, 8, mo_conf.CAN_MO_CONF_Data);
		CAN_TX_Publish(nh, aggm_pub, CAN_ID_MO_VAL, 8, mo_val.CAN_MO_VAL_Data);
		CAN_Control_report_Publish(nh, aggm_CCAN_pub);
		CAN_Info_Publish(nh2, aggm_pub4);
		gps_msg.header.stamp = ros::Time::now();
		aggm_pub5.publish(gps_msg);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	

	
}

void Print_for_Test(void)
{
	//ROS_INFO("%u,%f,%u,%u,%d,%d",mo_val.Str.Accel_Dec_Cmd,Acc_value,rpt_acc.Str.ACC_Fd_VSpeed,rpt_acc.Str.ACC_Fd_SetSpeed,rpt_acc.Str.LONG_ACCEL,rpt_acc.Str.ACC_Fd_Ovrrd_Rturn_sig);
	//ROS_INFO("MO_CONF:%X %X %X %X %X %X %X %X",mo_conf.CAN_MO_CONF_Data[0],mo_conf.CAN_MO_CONF_Data[1],mo_conf.CAN_MO_CONF_Data[2],mo_conf.CAN_MO_CONF_Data[3],mo_conf.CAN_MO_CONF_Data[4],mo_conf.CAN_MO_CONF_Data[5],mo_conf.CAN_MO_CONF_Data[6],mo_conf.CAN_MO_CONF_Data[7]);
	//ROS_INFO("MO_VAL :%X %X %X %X %X %X %X %X",mo_val.CAN_MO_VAL_Data[0],mo_val.CAN_MO_VAL_Data[1],mo_val.CAN_MO_VAL_Data[2],mo_val.CAN_MO_VAL_Data[3],mo_val.CAN_MO_VAL_Data[4],mo_val.CAN_MO_VAL_Data[5],mo_val.CAN_MO_VAL_Data[6],mo_val.CAN_MO_VAL_Data[7]);

	//ROS_INFO("RPT_ACC:%X %X %X %X %X %X %X %X",rpt_acc.CAN_REPORT_ACC_Data[0],rpt_acc.CAN_REPORT_ACC_Data[1],rpt_acc.CAN_REPORT_ACC_Data[2],rpt_acc.CAN_REPORT_ACC_Data[3],rpt_acc.CAN_REPORT_ACC_Data[4],rpt_acc.CAN_REPORT_ACC_Data[5],rpt_acc.CAN_REPORT_ACC_Data[6],rpt_acc.CAN_REPORT_ACC_Data[7]);
	//ROS_INFO("RPT_EPS:%X %X %X %X %X %X %X %X",rpt_eps.CAN_REPORT_EPS_Data[0],rpt_eps.CAN_REPORT_EPS_Data[1],rpt_eps.CAN_REPORT_EPS_Data[2],rpt_eps.CAN_REPORT_EPS_Data[3],rpt_eps.CAN_REPORT_EPS_Data[4],rpt_eps.CAN_REPORT_EPS_Data[5],rpt_eps.CAN_REPORT_EPS_Data[6],rpt_eps.CAN_REPORT_EPS_Data[7]);
		
	//ROS_INFO("%u",rpt_acc.Str.ACC_Fd_VSpeed);
	//ROS_INFO("%X %X",gway1.Str.Gway_Wheel_Velocity_FL_H,gway1.Str.Gway_Wheel_Velocity_FL_L);
	//ROS_INFO("FL:%f ,FR:%f ,RL:%f ,RR:%f", FL, FR, RL, RR);
	//ROS_INFO("FL:%f ,FR:%f ,RL:%f ,RR:%f", (double)gway1.Str.Gway_Wheel_Velocity_FL*0.03125,(double)gway1.Str.Gway_Wheel_Velocity_FR*0.03125,(double)gway1.Str.Gway_Wheel_Velocity_RL*0.03125,(double)gway1.Str.Gway_Wheel_Velocity_RR*0.03125);
	//ROS_INFO("TQ:%f ,SP:%u ,AG:%f, AC:%u ,PB:%u ,LTSP:%f", St_Tq,gway2.Str.Gway_Steering_Speed*4, StAngle,gway2.Str.Gway_AirConditioner_On,gway2.Str.Gway_Parking_Brake_Active, Lat_Acc_Speed); 
	//ROS_INFO("TP:%u ,GD:%X ,GT:%X ,RPM:%f ,BCP:%f , BA:%X ,APP:%f",throttle_po, gway3.Str.Gway_GearSelDisp, gway3.Str.Gway_Gear_Target_Change,engine_RPM, brake_press, gway3.Str.Gway_Brake_Active, (double)gway3.Str.Gway_Accel_Pedal_Position * 0.3906);
	//ROS_INFO("YR:%f ,SP:%u ,ACC:%f ,OD:%f", Yaw_rate, Vspeed, Lon_Acc_Speed, Odometer);
	//ROS_INFO("GW5:%X %X %X %X %X %X %X %X",gway5.CAN_GWAY5_Data[0],gway5.CAN_GWAY5_Data[1],gway5.CAN_GWAY5_Data[2],gway5.CAN_GWAY5_Data[3],gway5.CAN_GWAY5_Data[4],gway5.CAN_GWAY5_Data[5],gway5.CAN_GWAY5_Data[6],gway5.CAN_GWAY5_Data[7]);
	//ROS_INFO(",%.2f,%.2f,%.2f,%.2f,%.2f",Dest_Speed, f_Vspeed, Speed_Error, Gain_P, PID_speed);
	
}


void Speed_Control_PID(void)
{
	Speed_Error = Dest_Speed - f_Vspeed;			// error = dest speed - current speed
	PID_speed = Speed_Error * Gain_P;

	if(PID_speed  < -3.0 )
		PID_speed = -3.0;
	else if(PID_speed > 1.0)
		PID_speed = 1.0;
}

void KBCallback(const local_planning::Keyboard_Interface::ConstPtr& msg)
{
//	ROS_INFO("%d",msg->mode);
	switch(msg->mode)
	{
		case 0:
			ROS_INFO("Both Disable");
			mo_conf.Str.EPS_En = 0;
			mo_conf.Str.ACC_En = 0;
			break;
		case BOTH_EN:
			ROS_INFO("Both Enable");
			mo_conf.Str.EPS_En = 1;
			mo_conf.Str.ACC_En = 1;
			break;		
		case EPS_EN:
			ROS_INFO("EPS Enable");
			mo_conf.Str.EPS_En = 1;
			break;
		case ACC_EN:
			ROS_INFO("ACC Enable");
			mo_conf.Str.ACC_En = 1;
			break;
		case CONTROL:
			//KEYBOARD_CONTROL(msg->mode);
			break;
		case SPEEDUP:
			Dest_Speed += 1;
			if(Dest_Speed > 10) Dest_Speed = 10;	
			//Acc_value += 0.1;
			//ROS_INFO("ACC:%f",Acc_value);
			break;
		case SPEEDDOWN:
			Dest_Speed -= 1;
			if(Dest_Speed < 0) Dest_Speed = 0;
			//Acc_value -= 0.1;
			//ROS_INFO("ACC:%f",Acc_value);
			break;
		case STRLEFT:
			Gain_P += 0.01;
			//Angle += 1;
			//ROS_INFO("st_angle:%f",Angle);
			break;
		case STRRIGHT:
			Gain_P -= 0.01;
			//Angle -= 1;
			//ROS_INFO("st_angle:%f",Angle);
			break;
	}
}


uint8_t test_buf[8];
void canmsgCallback(const can_msgs::Frame::ConstPtr& Lmsg)
{
	//ROS_INFO("%x", Lmsg->id);
	switch(Lmsg->id)
	{
		case CAN_ID_REPORT_EPS:
			memcpy(rpt_eps.CAN_REPORT_EPS_Data, &Lmsg->data[0], sizeof(test_buf));
			RPT_EPS_T.EPS_Fd_En = rpt_eps.Str.EPS_Fd_En;
			RPT_EPS_T.Mo_Fd_State = rpt_eps.Str.Mo_Fd_State;
			RPT_EPS_T.Mo_Fd_Seq = rpt_eps.Str.Mo_Fd_Seq;
			RPT_EPS_T.EPS_Fd_ErrBit_PD = rpt_eps.Str.EPS_Fd_ErrBit_PD;
			RPT_EPS_T.EPS_Fd_ErrBit_EPS = rpt_eps.Str.EPS_Fd_ErrBit_EPS;
			RPT_EPS_T.EPS_Fd_ErrBit_Vinfo = rpt_eps.Str.EPS_Fd_ErrBit_Vinfo;
			RPT_EPS_T.EPS_Fd_ErrBit_SAS = rpt_eps.Str.EPS_Fd_ErrBit_SAS;
			RPT_EPS_T.EPS_Fd_SteerAngle = rpt_eps.Str.EPS_Fd_SteerAngle * 0.1;
                        RPT_EPS_T.EPS_Fd_Steer_Torq = (rpt_eps.Str.EPS_Fd_Steer_Torq - 0) * 0.01;
                        RPT_EPS_T.EPS_Fd_Str_out_tq = (rpt_eps.Str.EPS_Fd_Str_out_tq - 0) * 0.1;
			RPT_EPS_T.Mo_Fd_AlvCnt = rpt_eps.Str.Mo_Fd_AlvCnt;
			break;
		case CAN_ID_REPORT_ACC:
			memcpy(rpt_acc.CAN_REPORT_ACC_Data, &Lmsg->data[0], sizeof(test_buf));
			RPT_ACC_T.ACC_Fd_En = rpt_acc.Str.ACC_Fd_En;
			RPT_ACC_T.Mo_Fd_State = rpt_acc.Str.Mo_Fd_State;
			RPT_ACC_T.Mo_Fd_Seq = rpt_acc.Str.Mo_Fd_Seq;
			RPT_ACC_T.ACC_Fd_ErrBit_PD = rpt_acc.Str.ACC_Fd_ErrBit_PD;
			RPT_ACC_T.ACC_Fd_ErrBit_TCU = rpt_acc.Str.ACC_Fd_ErrBit_TCU;
			RPT_ACC_T.ACC_Fd_ErrBit_ACC = rpt_acc.Str.ACC_Fd_ErrBit_ACC;
			RPT_ACC_T.ACC_Fd_aeb_seq = rpt_acc.Str.ACC_Fd_aeb_seq;
			RPT_ACC_T.ACC_Fd_VSpeed = rpt_acc.Str.ACC_Fd_VSpeed;
			RPT_ACC_T.ACC_Fd_SetSpeed = rpt_acc.Str.ACC_Fd_SetSpeed;
                        RPT_ACC_T.LONG_ACCEL = (rpt_acc.Str.LONG_ACCEL - 0) * 0.01;
			RPT_ACC_T.ACC_Fd_Ovrrd_Rturn_sig = rpt_acc.Str.ACC_Fd_Ovrrd_Rturn_sig;
			RPT_ACC_T.ACC_Fd_Ovrrd_Haz_sig = rpt_acc.Str.ACC_Fd_Ovrrd_Haz_sig;
			RPT_ACC_T.ACC_Fd_Ovrrd_Lturn_sig = rpt_acc.Str.ACC_Fd_Ovrrd_Lturn_sig;
			RPT_ACC_T.G_SEL_DISP = rpt_acc.Str.G_SEL_DISP;
			RPT_ACC_T.Mo_Fd_AlvCnt = rpt_acc.Str.Mo_Fd_AlvCnt;
			break;
		//case CAN_ID_TEST:
			//ROS_INFO("0x210 Data get");
			//ROS_INFO("%x",Lmsg->data[0]);
			//memcpy(test_buf, &Lmsg->data[0], sizeof(rpt_eps.CAN_REPORT_EPS_Data+8));
			//ROS_INFO("%X %X %X %X %X %X %X %X ",Lmsg->data[0],Lmsg->data[1],Lmsg->data[2],
			//Lmsg->data[3],Lmsg->data[4],Lmsg->data[5],Lmsg->data[6],Lmsg->data[7]);
		//	break;
	}
}

void canmsg2Callback(const can_msgs::Frame::ConstPtr& L2msg)
{
	//ROS_INFO("%x", L2msg->id);
	switch(L2msg->id)
	{
		case CAN_ID_GWAY1:
			memcpy(gway1.CAN_GWAY1_Data, &L2msg->data[0], sizeof(test_buf));
			FL = (float)((int16_t)((gway1.Str.Gway_Wheel_Velocity_FL_H << 8) + gway1.Str.Gway_Wheel_Velocity_FL_L));
			FL = FL * 0.03125;
			FR = (float)((gway1.Str.Gway_Wheel_Velocity_FR_H << 8) + gway1.Str.Gway_Wheel_Velocity_FR_L);
			FR = FR * 0.03125;
			RL = (float)((gway1.Str.Gway_Wheel_Velocity_RL_H << 8) + gway1.Str.Gway_Wheel_Velocity_RL_L);
			RL = RL * 0.03125;
			RR = (float)((gway1.Str.Gway_Wheel_Velocity_RR_H << 8) + gway1.Str.Gway_Wheel_Velocity_RR_L);
			RR = RR * 0.03125;

			//ROS_INFO("GW1:%X %X %X %X %X %X %X %X",gway1.CAN_GWAY1_Data[0],gway1.CAN_GWAY1_Data[1],gway1.CAN_GWAY1_Data[2],gway1.CAN_GWAY1_Data[3],gway1.CAN_GWAY1_Data[4],gway1.CAN_GWAY1_Data[5],gway1.CAN_GWAY1_Data[6],gway1.CAN_GWAY1_Data[7]);
			break;
		case CAN_ID_GWAY2:
			memcpy(gway2.CAN_GWAY2_Data, &L2msg->data[0], sizeof(test_buf));
			Lat_Acc_Speed = ((float)((gway2.Str.Gway_Lateral_Accel_Speed_H << 8) + gway2.Str.Gway_Lateral_Accel_Speed_L))*0.01 -10.23;
			Parking_brake_active = (uint8_t)gway2.Str.Gway_Parking_Brake_Active;
			Air_cond_on = (uint8_t)gway2.Str.Gway_AirConditioner_On;
			StAngle = ((float)((int16_t)((gway2.Str.Gway_Steering_Angle_H << 8) + gway2.Str.Gway_Steering_Angle_L)))*0.1;
			St_angular = ((uint16_t)gway2.Str.Gway_Steering_Speed) * 4;
			St_Tq = ((float)(((gway2.Str.Gway_Steering_Tq_H << 8) + gway2.Str.Gway_Steering_Tq_L)-0x800))*0.01; 

			//ROS_INFO("GW2:%X %X %X %X %X %X %X %X",gway2.CAN_GWAY2_Data[0],gway2.CAN_GWAY2_Data[1],gway2.CAN_GWAY2_Data[2],gway2.CAN_GWAY2_Data[3],gway2.CAN_GWAY2_Data[4],gway2.CAN_GWAY2_Data[5],gway2.CAN_GWAY2_Data[6],gway2.CAN_GWAY2_Data[7]);
			break;
		case CAN_ID_GWAY3:
			memcpy(gway3.CAN_GWAY3_Data, &L2msg->data[0], sizeof(test_buf));
			acc_pedal_pos = ((float)(gway3.Str.Gway_Accel_Pedal_Position)) * 0.3906;
			brake_active = (uint8_t)(gway3.Str.Gway_Brake_Active);
			Gear_target_change = (uint8_t)(gway3.Str.Gway_Gear_Target_Change);
			GearSelDisp = (uint8_t)(gway3.Str.Gway_GearSelDisp);
			throttle_po = (uint8_t)(((float)(((gway3.Str.Gway_Throttle_Position_H << 4) + gway3.Str.Gway_Throttle_Position_L)- 0x20)) * 100/213);	
			engine_speed = (float)((gway3.Str.Gway_Engine_Speed_H << 12) + (gway3.Str.Gway_Engine_Speed_M << 4) + gway3.Str.Gway_Engine_Speed_L);
			engine_speed = engine_speed * 0.25;
			brake_press = ((float)((gway3.Str.Gway_BrakeMasterCylinder_Pressure_H << 12) + (gway3.Str.Gway_BrakeMasterCylinder_Pressure_H << 4) + gway3.Str.Gway_BrakeMasterCylinder_Pressure_L)) * 0.1;
			//ROS_INFO("GW3:%X %X %X %X %X %X %X %X",gway3.CAN_GWAY3_Data[0],gway3.CAN_GWAY3_Data[1],gway3.CAN_GWAY3_Data[2],gway3.CAN_GWAY3_Data[3],gway3.CAN_GWAY3_Data[4],gway3.CAN_GWAY3_Data[5],gway3.CAN_GWAY3_Data[6],gway3.CAN_GWAY3_Data[7]);
			break;
		case CAN_ID_GWAY4:
			memcpy(gway4.CAN_GWAY4_Data, &L2msg->data[0], sizeof(test_buf));
			Yaw_rate = ((float)((gway4.Str.Gway_Yaw_Rate_Sensor_H << 8) + gway4.Str.Gway_Yaw_Rate_Sensor_L))*0.01 - 40.95;
			Vspeed = (uint8_t)gway4.Str.Gway_Vehicle_Speed_Engine;
			Lon_Acc_Speed = ((float)((gway4.Str.Gway_Longitudinal_Accel_Speed_H << 8) + gway4.Str.Gway_Longitudinal_Accel_Speed_L)) * 0.01 - 10.23;
			Odometer = ((float)((gway4.Str.Gway_Cluster_Odometer_H << 16) + (gway4.Str.Gway_Cluster_Odometer_M << 8) + gway4.Str.Gway_Cluster_Odometer_L)) * 0.1;
			//ROS_INFO("GW4:%X %X %X %X %X %X %X %X",gway4.CAN_GWAY4_Data[0],gway4.CAN_GWAY4_Data[1],gway4.CAN_GWAY4_Data[2],gway4.CAN_GWAY4_Data[3],gway4.CAN_GWAY4_Data[4],gway4.CAN_GWAY4_Data[5],gway4.CAN_GWAY4_Data[6],gway4.CAN_GWAY4_Data[7]);
			break;
		case CAN_ID_GWAY5:
			memcpy(gway5.CAN_GWAY5_Data, &L2msg->data[0], sizeof(test_buf));
			//ROS_INFO("GW5:%X %X %X %X %X %X %X %X",gway5.CAN_GWAY5_Data[0],gway5.CAN_GWAY5_Data[1],gway5.CAN_GWAY5_Data[2],gway5.CAN_GWAY5_Data[3],gway5.CAN_GWAY5_Data[4],gway5.CAN_GWAY5_Data[5],gway5.CAN_GWAY5_Data[6],gway5.CAN_GWAY5_Data[7]);
			break;
		case CAN_ID_MsgLat:
			 msgLatitude = *(double *)(&L2msg->data[0]);
                         gps_msg.Latitude = msgLatitude ;//* 0.01754444;
			break;
		case CAN_ID_MsgLon:
			msgLongitude = *(double *)(&L2msg->data[0]);
                        gps_msg.Longitude = msgLongitude;// * 0.01754444;
			break;
		case CAN_ID_MsgYaw:
			msgHeading = *(float *)(&L2msg->data[0]);
                        gps_msg.Heading = msgHeading * 0.01754444;
			break;
		case CAN_ID_MsgState:
			GPSQuality = *(uint16_t *)(&L2msg->data[0]);
			LatStd = *(uint16_t *)(&L2msg->data[2]);
			LonStd = *(uint16_t *)(&L2msg->data[4]);
			YawStd = *(uint16_t *)(&L2msg->data[6]);
			gps_msg.GPSQuality = GPSQuality;
			gps_msg.LatStd = LatStd;
			gps_msg.LonStd = LonStd;
			gps_msg.HeadingStd = YawStd;
			break;


		//case CAN_ID_TEST:
			//ROS_INFO("0x210 Data get");
			//ROS_INFO("%x",Lmsg->data[0]);
				//memcpy(test_buf, &L2msg->data[0], sizeof(test_buf));
			//ROS_INFO("%X %X %X %X %X %X %X %X ",L2msg->data[0],L2msg->data[1],L2msg->data[2],
			//		L2msg->data[3],L2msg->data[4],L2msg->data[5],L2msg->data[6],L2msg->data[7]);
		//	break;
	}
}


/**
  @date 18-09-07
  @author elfmawang
  @brief   control can publish
  @param   NodeHandle, publish
  @return  
  */

uint8_t CAN_Control_report_Publish(ros::NodeHandle n, ros::Publisher pub)
{
	rpt_CCAN_msg.header.stamp = ros::Time::now();
	rpt_CCAN_msg.EPS_Fd_En = RPT_EPS_T.EPS_Fd_En;
	rpt_CCAN_msg.Mo_Fd_State = RPT_EPS_T.Mo_Fd_State;
	rpt_CCAN_msg.Mo_Fd_Seq = RPT_EPS_T.Mo_Fd_Seq;
	rpt_CCAN_msg.EPS_Fd_ErrBit_PD =	RPT_EPS_T.EPS_Fd_ErrBit_PD;
	rpt_CCAN_msg.EPS_Fd_ErrBit_EPS = RPT_EPS_T.EPS_Fd_ErrBit_EPS;
	rpt_CCAN_msg.EPS_Fd_ErrBit_Vinfo = RPT_EPS_T.EPS_Fd_ErrBit_Vinfo;
	rpt_CCAN_msg.EPS_Fd_ErrBit_SAS = RPT_EPS_T.EPS_Fd_ErrBit_SAS;
	rpt_CCAN_msg.EPS_Fd_SteerAngle = RPT_EPS_T.EPS_Fd_SteerAngle;
	rpt_CCAN_msg.EPS_Fd_Steer_Torq = RPT_EPS_T.EPS_Fd_Steer_Torq;
 	rpt_CCAN_msg.EPS_Fd_Str_out_tq = RPT_EPS_T.EPS_Fd_Str_out_tq;
	rpt_CCAN_msg.Mo_Fd_AlvCnt = RPT_EPS_T.Mo_Fd_AlvCnt;
	rpt_CCAN_msg.ACC_Fd_En = RPT_ACC_T.ACC_Fd_En;
	rpt_CCAN_msg.ACC_Fd_ErrBit_PD = RPT_ACC_T.ACC_Fd_ErrBit_PD;
	rpt_CCAN_msg.ACC_Fd_ErrBit_TCU = RPT_ACC_T.ACC_Fd_ErrBit_TCU;
	rpt_CCAN_msg.ACC_Fd_ErrBit_ACC = RPT_ACC_T.ACC_Fd_ErrBit_ACC;
	rpt_CCAN_msg.ACC_Fd_aeb_seq = RPT_ACC_T.ACC_Fd_aeb_seq;
	rpt_CCAN_msg.ACC_Fd_VSpeed = RPT_ACC_T.ACC_Fd_VSpeed;
	rpt_CCAN_msg.ACC_Fd_SetSpeed = RPT_ACC_T.ACC_Fd_SetSpeed;
	rpt_CCAN_msg.LONG_ACCEL = RPT_ACC_T.LONG_ACCEL;
	rpt_CCAN_msg.ACC_Fd_Ovrrd_Rturn_sig = RPT_ACC_T.ACC_Fd_Ovrrd_Rturn_sig;
	rpt_CCAN_msg.ACC_Fd_Ovrrd_Haz_sig = RPT_ACC_T.ACC_Fd_Ovrrd_Haz_sig;
	rpt_CCAN_msg.ACC_Fd_Ovrrd_Lturn_sig = RPT_ACC_T.ACC_Fd_Ovrrd_Lturn_sig;
	rpt_CCAN_msg.G_SEL_DISP = RPT_ACC_T.G_SEL_DISP;
	pub.publish(rpt_CCAN_msg);
}


/**
  @date 18-07-20
  @author elfmawang
  @brief   Can Tx publish function
  @param   NodeHandle, publish, can_id, dlc, data
  @return  
  */
uint8_t CAN_TX_Publish(ros::NodeHandle n, ros::Publisher pub, uint16_t id, uint8_t dlc, uint8_t* data)
{
	msg.header.frame_id = "0";
	msg.id = id;
	msg.dlc = dlc;
	std::copy(data, data+8, msg.data.begin()); 
	msg.header.stamp = ros::Time::now();
	pub.publish(msg);
}

/**
  @date 18-09-04
  @author elfmawang
  @brief   CAN Info Publisher
  @param   NodeHandle, publish
  @return  
  */

uint8_t CAN_Info_Publish(ros::NodeHandle n, ros::Publisher pub)
{
	msg_CAN.header.stamp = ros::Time::now();
	msg_CAN.speedfl = FL;
	msg_CAN.speedfr = FR;
	msg_CAN.speedrl = RL;
	msg_CAN.speedrr = RR;
	msg_CAN.speed_avr_r = (RL + RR)/2;
	
	msg_CAN.lat_acc_speed = Lat_Acc_Speed;
	msg_CAN.parking_brake_on = Parking_brake_active;
	msg_CAN.air_cond_on	= Air_cond_on;
	msg_CAN.steering_wheel_angle = StAngle;
	msg_CAN.steering_wheel_angular = St_angular;
	msg_CAN.steering_wheel_tq = St_Tq; 

	msg_CAN.pos_acc_pedal = acc_pedal_pos;
	msg_CAN.brake_active_state = brake_active;
	msg_CAN.gear_trans_state = Gear_target_change;
	msg_CAN.gear_seldisp = GearSelDisp;
	msg_CAN.pos_throttle = throttle_po;	
	msg_CAN.engine_speed = engine_speed;
	msg_CAN.brake_cylinder_press = brake_press;

	msg_CAN.vehicle_speed_engine = Vspeed;
	msg_CAN.lon_acc_speed = Lon_Acc_Speed;
	msg_CAN.cluster_odometer = Odometer;
	msg_CAN.yaw_rate = Yaw_rate;
	pub.publish(msg_CAN);
}

void kusvcmdCallback(const local_planning::kusv_ControlCmd::ConstPtr& cmdmsg)
{
	Dest_Speed = cmdmsg->kusv_linear_x;
	Angle = cmdmsg->kusv_angular_z;
}
