/**
  @file  		CAN2TopicConverter.cpp
  @brief 		CAN to Topic Converter

  @author  		elfmawang (kjh)
  @E-mail		kjh23135 (at) gmail.com
  @internal
  |+--Created		18-07-14
  |+--Compiler          ROS
  |+--Company		Konkuk Univ. Control Research Lab
  |+--Copyright  	Copyright (c) 2018, elfmawang

  @Revision
  =====================================================================
  */

#include <ros/ros.h>
#include <can_msgs/Frame.h>
#include "kusv_msgs/kusv_CanInfo.h"
#include "pose_estimation/DR_data.h"
#include <string>
#include <arpa/inet.h>
#include <math.h>
#include <iostream>

#define CAN_ID_GWAY1		0x100
#define CAN_ID_GWAY2		0x101
#define CAN_ID_GWAY3		0x102
#define CAN_ID_GWAY4		0x103
#define CAN_ID_GWAY5		0x123

// Structure Define
#pragma pack(push, 1)
//-------------- CAN GW

typedef union _CAN_MSG_GWAY1_
{
    uint8_t CAN_GWAY1_Data[8];
    struct{
        uint8_t Gway_Wheel_Velocity_FR_L;
        uint8_t Gway_Wheel_Velocity_FR_H        	: 6;
        uint8_t reserve1                                : 2;
        uint8_t Gway_Wheel_Velocity_RL_L;
        uint8_t Gway_Wheel_Velocity_RL_H        	: 6;
        uint8_t reserve2                                : 2;
        uint8_t Gway_Wheel_Velocity_RR_L;
        uint8_t Gway_Wheel_Velocity_RR_H                : 6;
        uint8_t reserve3                                : 2;
        uint8_t Gway_Wheel_Velocity_FL_L;
        uint8_t Gway_Wheel_Velocity_FL_H                : 6;
        uint8_t reserve4                                : 2;
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
		uint8_t Gway_Engine_Speed_L			: 4;
		uint8_t Gway_Engine_Speed_M;
		uint8_t Gway_Engine_Speed_H			: 4;
		uint8_t Gway_Gear_Target_Change			: 4;
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
#pragma pack(pop)



CAN_MSG_GWAY1 gway1;
CAN_MSG_GWAY2 gway2;
CAN_MSG_GWAY3 gway3;
CAN_MSG_GWAY4 gway4;
CAN_MSG_GWAY5 gway5;



// publish
uint8_t data_msg[8];
can_msgs::Frame msg;
kusv_msgs::kusv_CanInfo msg_CAN;
pose_estimation::DR_data msg_DR_data;

//************************//
// VARIABLE & SIGNAL NAME //
//************************//
//********************************************************************************//
float FL, FR, RL, RR;                       // Wheel_Velocity

float Lat_Acc_Speed;                        // Lateral_Accel_Speed
float Parking_Brake_Active;                 // Parking_Brake_Active
float Air_Cond_On;                          // AirConditioiner_On
float StAngle;                              // Steering_Angle
float St_Angular_Vel;                       // Steering_Speed
uint8_t St_Tq;                              // Steering_Tq

float Acc_Pedal_Pose;                       // Accel_Pedal_Position
uint8_t Brake_Active;                       // Brake_Active
uint8_t Brake_Press;                        // BrakeMasterCylinder_Pressure
uint8_t Engine_Speed;                       // Engine_Speed
uint8_t Gear_Target_Change;                 // Gear_Target_Change
uint8_t GearSelDisp;                        // GearSelDisp
uint16_t Throttle_Pose;                     // Throttle_Position

float Odometer;                             // Cluster_Odometer
uint8_t Lon_Acc_Speed;                      // Longitudinal_Accel_Speed
float Vspeed;                               // Vehicle_Speed_Engine
float Yaw_rate;                             // Yaw_Rate_Sensor



//********************************************************************************//


// function
void canmsg_Callback(const can_msgs::Frame::ConstPtr& L_msg);
uint8_t CAN_TX_Publish(ros::NodeHandle n, ros::Publisher pub, uint16_t id, uint8_t dlc, uint8_t* data);
uint8_t DR_data_Publish(ros::NodeHandle n, ros::Publisher pub);
void Print_for_Test(void);

int main(int argc, char** argv)
{
        ros::init(argc, argv, "CAN2TopicConverter");
        ros::NodeHandle nh_1;
        ros::NodeHandle nh_2;
        ros::NodeHandle nh_3;

        ros::Publisher ros_can_pub = nh_1.advertise<pose_estimation::DR_data>("ros_can/DR_data", 500);    // can Tx setting (In person side)
        /*ros::Publisher ros_can_pub_2 = nh_3.advertise<ros_can::kusv_CanInfo>("kusv_CanInfo", 100);*/
        ros::Subscriber ros_can_sub = nh_2.subscribe("can_tx", 500, canmsg_Callback);                  // can Rx setting (In person side)

        ros::AsyncSpinner spinner(4);                          // use 3 threads 1. can tx, 2. can rx, 3. Keyboard_Interface  // can rx2
	
        ros::Rate loop_rate(50);                                                                       // Looping 50Hz
	spinner.start();

	while(ros::ok())
	{
                DR_data_Publish(nh_1, ros_can_pub); // Publish Dead Reckoning data

		ros::spinOnce();
		loop_rate.sleep();
	}
        return 0;
}

uint8_t test_buf[8];

void canmsg_Callback(const can_msgs::Frame::ConstPtr& L_msg)
{
    if (L_msg->id <= CAN_ID_GWAY5 && L_msg->id >= CAN_ID_GWAY1) {
        //ROS_INFO("\n[%d] ID: 0x%x", L_msg->header.seq, L_msg->id);
        switch(L_msg->id)
        {
                case CAN_ID_GWAY1:
                        memcpy(gway1.CAN_GWAY1_Data, &L_msg->data[0], sizeof(test_buf));
                        FL = (float)((int16_t)((gway1.Str.Gway_Wheel_Velocity_FL_H << 8) + gway1.Str.Gway_Wheel_Velocity_FL_L));
                        FL = FL * (0.03125);
                        FR = (float)((gway1.Str.Gway_Wheel_Velocity_FR_H << 8) + gway1.Str.Gway_Wheel_Velocity_FR_L);
                        FR = FR * (0.03125);
                        RL = (float)((gway1.Str.Gway_Wheel_Velocity_RL_H << 8) + gway1.Str.Gway_Wheel_Velocity_RL_L);
                        RL = RL * (0.03125);
                        RR = (float)((gway1.Str.Gway_Wheel_Velocity_RR_H << 8) + gway1.Str.Gway_Wheel_Velocity_RR_L);
                        RR = RR * (0.03125);

                        //ROS_INFO("GW1:%X %X %X %X %X %X %X %X",gway1.CAN_GWAY1_Data[0],gway1.CAN_GWAY1_Data[1],gway1.CAN_GWAY1_Data[2],gway1.CAN_GWAY1_Data[3],gway1.CAN_GWAY1_Data[4],gway1.CAN_GWAY1_Data[5],gway1.CAN_GWAY1_Data[6],gway1.CAN_GWAY1_Data[7]);
                        //ROS_INFO("\n FR: %f\n RL: %f\n RR: %f\n FL: %f", FR, RL, RR, FL);
                        ROS_INFO_STREAM("speed_avg_rear: "<< (RR+RL)/2);
                        break;

                case CAN_ID_GWAY2:
			memcpy(gway2.CAN_GWAY2_Data, &L_msg->data[0], sizeof(test_buf));
			Lat_Acc_Speed = ((float)((gway2.Str.Gway_Lateral_Accel_Speed_H << 8) + gway2.Str.Gway_Lateral_Accel_Speed_L))*0.01 -10.23;
			Parking_Brake_Active = (uint8_t)gway2.Str.Gway_Parking_Brake_Active;
			Air_Cond_On = (uint8_t)gway2.Str.Gway_AirConditioner_On;
			StAngle = ((float)((int16_t)((gway2.Str.Gway_Steering_Angle_H << 8) + gway2.Str.Gway_Steering_Angle_L)))*0.1;
			St_Angular_Vel = ((uint16_t)gway2.Str.Gway_Steering_Speed) * 4;
			St_Tq = ((float)(((gway2.Str.Gway_Steering_Tq_H << 8) + gway2.Str.Gway_Steering_Tq_L)-0x800))*0.01;
			
			//ROS_INFO("GW2:%X %X %X %X %X %X %X %X",gway2.CAN_GWAY2_Data[0],gway2.CAN_GWAY2_Data[1],gway2.CAN_GWAY2_Data[2],gway2.CAN_GWAY2_Data[3],gway2.CAN_GWAY2_Data[4],gway2.CAN_GWAY2_Data[5],gway2.CAN_GWAY2_Data[6],gway2.CAN_GWAY2_Data[7]); 


                        break;

                case CAN_ID_GWAY3:
                        memcpy(gway3.CAN_GWAY3_Data, &L_msg->data[0], sizeof(test_buf));
			Acc_Pedal_Pose = ((float)(gway3.Str.Gway_Accel_Pedal_Position)) * 0.3906;
			Brake_Active = (uint8_t)(gway3.Str.Gway_Brake_Active);
			Gear_Target_Change = (uint8_t)(gway3.Str.Gway_Gear_Target_Change);
			GearSelDisp = (uint8_t)(gway3.Str.Gway_GearSelDisp);
			Throttle_Pose = (uint8_t)(((float)(((gway3.Str.Gway_Throttle_Position_H << 4) + gway3.Str.Gway_Throttle_Position_L)- 0x20)) * 100/213);	
			Engine_Speed = (float)((gway3.Str.Gway_Engine_Speed_H << 12) + (gway3.Str.Gway_Engine_Speed_M << 4) + gway3.Str.Gway_Engine_Speed_L);
			Engine_Speed = Engine_Speed * 0.25;
			Brake_Press = ((float)((gway3.Str.Gway_BrakeMasterCylinder_Pressure_H << 12) + (gway3.Str.Gway_BrakeMasterCylinder_Pressure_H << 4) + gway3.Str.Gway_BrakeMasterCylinder_Pressure_L)) * 0.1;
			//ROS_INFO("GW3:%X %X %X %X %X %X %X %X",gway3.CAN_GWAY3_Data[0],gway3.CAN_GWAY3_Data[1],gway3.CAN_GWAY3_Data[2],gway3.CAN_GWAY3_Data[3],gway3.CAN_GWAY3_Data[4],gway3.CAN_GWAY3_Data[5],gway3.CAN_GWAY3_Data[6],gway3.CAN_GWAY3_Data[7]);
                        break;

                case CAN_ID_GWAY4:
                        memcpy(gway4.CAN_GWAY4_Data, &L_msg->data[0], sizeof(test_buf));
			Yaw_rate = ((float)((gway4.Str.Gway_Yaw_Rate_Sensor_H << 8) + gway4.Str.Gway_Yaw_Rate_Sensor_L))*0.01 - 40.95;
			Vspeed = (uint8_t)gway4.Str.Gway_Vehicle_Speed_Engine;
			Lon_Acc_Speed = ((float)((gway4.Str.Gway_Longitudinal_Accel_Speed_H << 8) + gway4.Str.Gway_Longitudinal_Accel_Speed_L)) * 0.01 - 10.23;
			Odometer = ((float)((gway4.Str.Gway_Cluster_Odometer_H << 16) + (gway4.Str.Gway_Cluster_Odometer_M << 8) + gway4.Str.Gway_Cluster_Odometer_L)) * 0.1;
                        //ROS_INFO("GW4:%X %X %X %X %X %X %X %X",gway4.CAN_GWAY4_Data[0],gway4.CAN_GWAY4_Data[1],gway4.CAN_GWAY4_Data[2],gway4.CAN_GWAY4_Data[3],gway4.CAN_GWAY4_Data[4],gway4.CAN_GWAY4_Data[5],gway4.CAN_GWAY4_Data[6],gway4.CAN_GWAY4_Data[7]);
                        ROS_INFO_STREAM("Yaw rate: " << Yaw_rate);

                        break;

                case CAN_ID_GWAY5:
                        memcpy(gway5.CAN_GWAY5_Data, &L_msg->data[0], sizeof(test_buf));
                        //ROS_INFO("GW5:%X %X %X %X %X %X %X %X",gway5.CAN_GWAY5_Data[0],gway5.CAN_GWAY5_Data[1],gway5.CAN_GWAY5_Data[2],gway5.CAN_GWAY5_Data[3],gway5.CAN_GWAY5_Data[4],gway5.CAN_GWAY5_Data[5],gway5.CAN_GWAY5_Data[6],gway5.CAN_GWAY5_Data[7]);

                        break;
        }
    }
}


/*
  @date 18-09-04
  @author elfmawang
  @brief   CAN Info Publisher
  @param   NodeHandle, publish
  @return  
*/

uint8_t DR_data_Publish(ros::NodeHandle n, ros::Publisher pub)
{
	msg_DR_data.header.stamp   = ros::Time::now();
	msg_DR_data.speed_avr_rear = (RL + RR)/2;
	msg_DR_data.yaw_rate       = Yaw_rate;

	pub.publish(msg_DR_data);
}

