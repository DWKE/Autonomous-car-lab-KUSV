#include <ros/ros.h>
#include <can_msgs/Frame.h>
#include "can/kusv_CanInfo.h"
#include <string>
#include <math.h>
#include <iostream>

// ------------------------------------------------------
// Define & Macro

// CAN ID
#define CAN_ID_GWAY1		0x100
#define CAN_ID_GWAY2		0x101
#define CAN_ID_GWAY3		0x102
#define CAN_ID_GWAY4		0x103
#define CAN_ID_GWAY5		0x123

// CAN structure Define

#pragma pack(push, 1)
//-------------- CAN GW

typedef union _CAN_MSG_GWAY1_
{
    uint8_t CAN_GWAY1_Data[8];
    struct{
        uint8_t Gway_Wheel_Velocity_FR_L;
        uint8_t Gway_Wheel_Velocity_FR_H              : 6;
        uint8_t reserve1                              : 2;
        uint8_t Gway_Wheel_Velocity_RL_L;
        uint8_t Gway_Wheel_Velocity_RL_H              : 6;
        uint8_t reserve2                              : 2;
        uint8_t Gway_Wheel_Velocity_RR_L;
        uint8_t Gway_Wheel_Velocity_RR_H              : 6;
        uint8_t reserve3                              : 2;
        uint8_t Gway_Wheel_Velocity_FL_L;
        uint8_t Gway_Wheel_Velocity_FL_H              : 6;
        uint8_t reserve4                              : 2;
    }Str;
}CAN_MSG_GWAY1;

typedef union _CAN_MSG_GWAY2_
{
    //***************TODO***************//
    uint8_t CAN_GWAY2_Data[8];
    struct{
        uint8_t Gway_Lateral_Accel_Speed_L;
        uint8_t Gway_Lateral_Accel_Speed_H;
        uint8_t Gway_Parking_Brake_Active             : 4;
        uint8_t Gway_AirConditioner_ON                : 4;
        uint8_t Gway_Steering_Angle_L;
        uint8_t Gway_Steering_Angle_H;
        uint8_t Gway_Steering_Speed;
        uint8_t Gway_Steering_Tq_L;
        uint8_t Gway_Steering_Tq_H;
    }Str;
}CAN_MSG_GWAY2;

typedef union _CAN_MSG_GWAY3_
{
    //***************TODO***************//
    uint8_t CAN_GWAY3_Data[8];
    struct{
        uint8_t Gway_Accel_Pedal_Position;
        uint8_t Gway_Brake_Active                     : 4;
        uint8_t Gway_BrakeMasterCylinder_Pressure_L   : 4;
        uint8_t Gway_BrakeMasterCylinder_Pressure;
        uint8_t Gway_BrakeMasterCylinder_Pressure_H   : 4;
        uint8_t Gway_Engine_Speed_L                   : 4;
        uint8_t Gway_Engine_Speed;
        uint8_t Gway_Engine_Speed_H                   : 4;
        uint8_t Gway_Gear_Target_Change               : 4;
        uint8_t Gway_GearSelDisp                      : 4;
        uint8_t Gway_Throttle_Position_L              : 4;
        uint8_t Gway_Throttle_Position_H              : 4;
        uint8_t reserve5                              : 4;
    }Str;
}CAN_MSG_GWAY3;

typedef union _CAN_MSG_GWAY4_
{
    //***************TODO***************//
    uint8_t CAN_GWAY4_Data[8];
    struct{
        uint8_t Gway_Cluster_Odometer_L;
        uint8_t Gway_Cluster_Odometer;
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
    //***************TODO***************//
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
// ------------------------------------------------------
// Variables

// CAN message
static CAN_MSG_GWAY1 gway1;
static CAN_MSG_GWAY2 gway2;
static CAN_MSG_GWAY3 gway3;
static CAN_MSG_GWAY4 gway4;
static CAN_MSG_GWAY5 gway5;

// VARIABLE & SIGNAL NAME //
static double st_dFL, st_dFR, st_dRL, st_dRR;                       // Wheel_Velocity

//***************TODO***************//
// add the signal variables
//GWAY2//
static double st_dLatAcc, st_dStAngle, st_dStTq;
static int st_iStSpeed, st_iAirCond, st_iPBrakeAct;
//GWAY3//
static double st_dAccPedPos, st_dBrakeCylPress, st_dEngSpd, st_dThPos;
static int st_iBrakeAct, st_iGearTarget, st_iGearSel;
//GWAY4//
static double st_dOdom, st_dLongAcc, st_dYawRate;
static int st_iSpeed;
//GWAY5//
static int st_iD0, st_iD1, st_iD2, st_iD3, st_iD4, st_iD5, st_iD6, st_iD7;

//********************************************************************************//


void canmsg_Callback(const can_msgs::Frame::ConstPtr& L_msg)
{
    uint8_t test_buf[8];

    if (L_msg->id <= CAN_ID_GWAY5 && L_msg->id >= CAN_ID_GWAY1) {
        ROS_INFO("\n[%d] ID: 0x%x", L_msg->header.seq, L_msg->id);
        switch(L_msg->id)
        {
        case CAN_ID_GWAY1:
            memcpy(gway1.CAN_GWAY1_Data, &L_msg->data[0], sizeof(test_buf));
            st_dFL = (double)((int16_t)(gway1.Str.Gway_Wheel_Velocity_FL_H << 8) + gway1.Str.Gway_Wheel_Velocity_FL_L);
            st_dFL = st_dFL * 0.03125;
            st_dFR = (double)((int16_t)(gway1.Str.Gway_Wheel_Velocity_FR_H << 8) + gway1.Str.Gway_Wheel_Velocity_FR_L);
            st_dFR = st_dFR * 0.03125;
            st_dRL = (double)((int16_t)(gway1.Str.Gway_Wheel_Velocity_RL_H << 8) + gway1.Str.Gway_Wheel_Velocity_RL_L);
            st_dRL = st_dRL * 0.03125;
            st_dRR = (double)((int16_t)(gway1.Str.Gway_Wheel_Velocity_RR_H << 8) + gway1.Str.Gway_Wheel_Velocity_RR_L);
            st_dRR = st_dRR * 0.03125;

            ROS_INFO("GW1:%X %X %X %X %X %X %X %X",gway1.CAN_GWAY1_Data[0],gway1.CAN_GWAY1_Data[1],gway1.CAN_GWAY1_Data[2],gway1.CAN_GWAY1_Data[3]
                    ,gway1.CAN_GWAY1_Data[4],gway1.CAN_GWAY1_Data[5],gway1.CAN_GWAY1_Data[6],gway1.CAN_GWAY1_Data[7]);
            ROS_INFO("FR: %f\t RL: %f\t RR: %f\t FL: %f", st_dFR, st_dRL, st_dRR, st_dFL);
            break;

        case CAN_ID_GWAY2:
            memcpy(gway2.CAN_GWAY2_Data, &L_msg->data[0], sizeof(test_buf));
            //***************TODO***************//
            st_dLatAcc = (double)((int16_t)(gway2.Str.Gway_Lateral_Accel_Speed_H << 8) + gway2.Str.Gway_Lateral_Accel_Speed_L);
            st_dLatAcc = (st_dLatAcc * 0.01) - 10.23;
            st_iPBrakeAct = gway2.Str.Gway_Parking_Brake_Active;
            st_iAirCond = gway2.Str.Gway_AirConditioner_ON;
            st_dStAngle = (double)((int16_t)(gway2.Str.Gway_Steering_Angle_H << 8) + gway2.Str.Gway_Steering_Angle_L);
            st_dStAngle = st_dStAngle * 0.1;
            st_iStSpeed = gway2.Str.Gway_Steering_Speed * 4;
            st_dStTq = (double)((int16_t)(gway2.Str.Gway_Steering_Tq_H << 8) + gway2.Str.Gway_Steering_Tq_L);
            st_dStTq = (st_dStTq - 0x800) * 0.01;
            ROS_INFO("GW2:%X %X %X %X %X %X %X %X",gway2.CAN_GWAY2_Data[0],gway2.CAN_GWAY2_Data[1],gway2.CAN_GWAY2_Data[2],gway2.CAN_GWAY2_Data[3]
                    ,gway2.CAN_GWAY2_Data[4],gway2.CAN_GWAY2_Data[5],gway2.CAN_GWAY2_Data[6],gway2.CAN_GWAY2_Data[7]);
            ROS_INFO("LatAcc: %f\t PBrakeAct: %d\t AirCond: %d\t StAngle: %f\t StSpeed: %d\t StTq: %f"
                     , st_dLatAcc, st_iPBrakeAct, st_iAirCond, st_dStAngle, st_iStSpeed, st_dStTq);
            break;

        case CAN_ID_GWAY3:
            memcpy(gway3.CAN_GWAY3_Data, &L_msg->data[0], sizeof(test_buf));
            //***************TODO***************//
            st_dAccPedPos = (double)(gway3.Str.Gway_Accel_Pedal_Position);
            st_dAccPedPos = st_dAccPedPos * 0.3906;
            st_iBrakeAct = gway3.Str.Gway_Brake_Active;
            st_dBrakeCylPress = (double)((int16_t)(gway3.Str.Gway_BrakeMasterCylinder_Pressure_H << 12) + (int16_t)(gway3.Str.Gway_BrakeMasterCylinder_Pressure << 4) + gway3.Str.Gway_BrakeMasterCylinder_Pressure_L);
            st_dBrakeCylPress = st_dBrakeCylPress * 0.1;
            st_dEngSpd = (double)((int16_t)(gway3.Str.Gway_Engine_Speed_H << 12) + (int16_t)(gway3.Str.Gway_Engine_Speed << 4) + gway3.Str.Gway_Engine_Speed_L);
            st_dEngSpd = st_dEngSpd * 0.25;
            st_iGearTarget = gway3.Str.Gway_Gear_Target_Change;
            st_iGearSel = gway3.Str.Gway_GearSelDisp;
            st_dThPos = (double)((int8_t)(gway3.Str.Gway_Throttle_Position_H << 4) + gway3.Str.Gway_Throttle_Position_L);
            st_dThPos = (st_dThPos - 0x20) * 100 / 213;
            ROS_INFO("GW3:%X %X %X %X %X %X %X %X",gway3.CAN_GWAY3_Data[0],gway3.CAN_GWAY3_Data[1],gway3.CAN_GWAY3_Data[2],gway3.CAN_GWAY3_Data[3]
                    ,gway3.CAN_GWAY3_Data[4],gway3.CAN_GWAY3_Data[5],gway3.CAN_GWAY3_Data[6],gway3.CAN_GWAY3_Data[7]);
            ROS_INFO("AccPedalPos: %f\t BrakeAct: %d\t BrakeCylPress: %f\t EngSpd: %f\t GearTarget: %d\t GearSel: %d\t, ThrottlePose: %f"
                     ,st_dAccPedPos, st_iBrakeAct, st_dBrakeCylPress, st_dEngSpd, st_iGearTarget, st_iGearSel, st_dThPos);
            break;

        case CAN_ID_GWAY4:
            memcpy(gway4.CAN_GWAY4_Data, &L_msg->data[0], sizeof(test_buf));
            //***************TODO***************//
            st_dOdom = (double)((gway4.Str.Gway_Cluster_Odometer_H << 16) + (gway4.Str.Gway_Cluster_Odometer << 8) + gway4.Str.Gway_Cluster_Odometer_L);
            st_dOdom = st_dOdom * 0.1;
            st_dLongAcc = (double)((int16_t)(gway4.Str.Gway_Longitudinal_Accel_Speed_H << 8) + gway4.Str.Gway_Longitudinal_Accel_Speed_L);
            st_dLongAcc = st_dLongAcc * 0.01 - 10.23;
            st_iSpeed = gway4.Str.Gway_Vehicle_Speed_Engine;
            st_dYawRate = (double)((int16_t)(gway4.Str.Gway_Yaw_Rate_Sensor_H << 8) + gway4.Str.Gway_Yaw_Rate_Sensor_L);
            st_dYawRate = st_dYawRate * 0.01 - 40.95;

            ROS_INFO("GW4:%X %X %X %X %X %X %X %X",gway4.CAN_GWAY4_Data[0],gway4.CAN_GWAY4_Data[1],gway4.CAN_GWAY4_Data[2],gway4.CAN_GWAY4_Data[3]
                    ,gway4.CAN_GWAY4_Data[4],gway4.CAN_GWAY4_Data[5],gway4.CAN_GWAY4_Data[6],gway4.CAN_GWAY4_Data[7]);
            ROS_INFO("Odom: %f\t LongAcc: %f\t Speed: %d\t YawRate: %f", st_dOdom, st_dLongAcc, st_iSpeed, st_dYawRate);
            break;

        case CAN_ID_GWAY5:
            memcpy(gway5.CAN_GWAY5_Data, &L_msg->data[0], sizeof(test_buf));
            //***************TODO***************//
            st_iD0 = gway5.Str.Dummy0;
            st_iD1 = gway5.Str.Dummy1;
            st_iD2 = gway5.Str.Dummy2;
            st_iD3 = gway5.Str.Dummy3;
            st_iD4 = gway5.Str.Dummy4;
            st_iD5 = gway5.Str.Dummy5;
            st_iD6 = gway5.Str.Dummy6;
            st_iD7 = gway5.Str.Dummy7;
            ROS_INFO("GW5:%X %X %X %X %X %X %X %X",gway5.CAN_GWAY5_Data[0],gway5.CAN_GWAY5_Data[1],gway5.CAN_GWAY5_Data[2],gway5.CAN_GWAY5_Data[3]
                    ,gway5.CAN_GWAY5_Data[4],gway5.CAN_GWAY5_Data[5],gway5.CAN_GWAY5_Data[6],gway5.CAN_GWAY5_Data[7]);
            ROS_INFO("Dummy0: %X\t Dummy1: %X\t Dummy2: %X\t Dummy3: %X\t Dummy4: %X\t Dummy5: %X\t Dummy6: %X\t Dummy7: %X"
                     , st_iD0, st_iD1, st_iD2, st_iD3, st_iD4, st_iD5, st_iD6, st_iD7);
            break;
        }
    }
}


uint8_t CAN_Info_Publish(ros::NodeHandle n, ros::Publisher pub)
{
    can::kusv_CanInfo msg_CAN_Signal;

    msg_CAN_Signal.header.stamp = ros::Time::now();
    msg_CAN_Signal.speedfl = st_dFL;
    msg_CAN_Signal.speedfr = st_dFR;
    msg_CAN_Signal.speedrl = st_dRL;
    msg_CAN_Signal.speedrr = st_dRR;
    msg_CAN_Signal.speed_avr_r = (st_dRL + st_dRR)/2;

    //***************TODO***************//
    //GWAY2//
    msg_CAN_Signal.lat_acc_speed = st_dLatAcc;
    msg_CAN_Signal.parking_brake_on = st_iPBrakeAct;
    msg_CAN_Signal.air_cond_on = st_iAirCond;
    msg_CAN_Signal.steering_wheel_angle = st_dStAngle;
    msg_CAN_Signal.steering_wheel_angular = st_iStSpeed;
    msg_CAN_Signal.steering_wheel_tq = st_dStTq;

    //GWAY3//
    msg_CAN_Signal.pos_acc_pedal = st_dAccPedPos;
    msg_CAN_Signal.brake_active_state = st_iBrakeAct;
    msg_CAN_Signal.brake_cylinder_press = st_dBrakeCylPress;
    msg_CAN_Signal.engine_speed = st_dEngSpd;
    msg_CAN_Signal.gear_trans_state = st_iGearTarget;
    msg_CAN_Signal.gear_seldisp = st_iGearSel;
    msg_CAN_Signal.pos_throttle = st_dThPos;
    //GWAY4//
    msg_CAN_Signal.cluster_odometer = st_dOdom;
    msg_CAN_Signal.lon_acc_speed = st_dLongAcc;
    msg_CAN_Signal.vehicle_speed_engine = st_iSpeed;
    msg_CAN_Signal.yaw_rate = st_dYawRate;

    //GWAY5//


    pub.publish(msg_CAN_Signal);
}



int main(int argc, char** argv)
{
    ros::init(argc, argv, "CAN_RX");
    ros::NodeHandle nh_pub;
    ros::NodeHandle nh_sub;

    ros::Publisher can_pub = nh_pub.advertise<can::kusv_CanInfo>("kusv_CanInfo", 100);
    ros::Subscriber can_sub = nh_sub.subscribe("can_rx", 500, canmsg_Callback);                  // can Rx setting (In person side)

    ros::AsyncSpinner spinner(4);                                                                  // use 3 threads 1. can tx, 2. can rx, 3. Keyboard_Interface  // can rx2

    ros::Rate loop_rate(50);                                                                       // Looping 50Hz
    spinner.start();

    while(ros::ok())
    {
        CAN_Info_Publish(nh_pub, can_pub);
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
