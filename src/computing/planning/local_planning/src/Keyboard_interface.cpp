/**
  @file  		Keyboard_interface.cpp
  @brief 		Make topic message using keyboard input

  @author  		elfmawang (kjh)
  @E-mail		kjh23135 (at) gmail.com
  @internal
  |+--Created		18-07-23
  |+--Compiler      gcc/g++
  |+--Company		Konkuk Univ. Control Research Lab
  |+--Copyright  	Copyright (c) 2018, elfmawang

  @Revision
  =====================================================================
  */


#include <ros/ros.h>
#include <termios.h>
#include "local_planning/Keyboard_Interface.h"
 
enum State 
{
	NONE
	, WAIT
	, BOTH_ON
	, EPS_ON
	, ACC_ON
	, CONTROL
	, CTRL_WASD
	, MAX_STATE
};


int state = NONE;
local_planning::Keyboard_Interface msg;

char getch();
void print_Menu(char c, const ros::Publisher pub);
void CTRL_WASD_switch(char c, const ros::Publisher pub);
void select_state(char c);
 
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "key_input_node");
    ros::NodeHandle n;
 
        ros::Publisher KBIF_pub = n.advertise<local_planning::Keyboard_Interface>("KBIF_msg", 100);
    ros::Rate loop_rate(100);


    while (ros::ok())
    {

		if(atoi(argv[1]) == 1)
		{
			msg.stamp = ros::Time::now();
			msg.mode = 1;
			KBIF_pub.publish(msg);
		}
		else if(atoi(argv[1]) == 0)
		{
			msg.stamp = ros::Time::now();
			msg.mode = 0;
			KBIF_pub.publish(msg);
		}
		else if(atoi(argv[1]) == 2)
		{
			msg.stamp = ros::Time::now();
			msg.mode = 2;
			KBIF_pub.publish(msg);
		}
		else if(atoi(argv[1]) == 3)
		{
			msg.stamp = ros::Time::now();
			msg.mode = 3;
			KBIF_pub.publish(msg);
		}
		
        //int c = 0;
        //c = getch();
		//print_Menu(c, KBIF_pub);
		//if(c != NULL)
			//ROS_INFO("%c", c);
 
        ros::spinOnce();
 
        loop_rate.sleep();
    }
 
    return 0;
}

char getch()
{
    fd_set set;
    struct timeval timeout;
    int rv;
    char buff = 0;
    int len = 1;
    int filedesc = 0;

    FD_ZERO(&set);
    FD_SET(filedesc, &set);
     
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
 
    rv = select(filedesc + 1, &set, NULL, NULL, &timeout);
 
    struct termios old = {0};
    if (tcgetattr(filedesc, &old) < 0)
        ROS_ERROR("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(filedesc, TCSANOW, &old) < 0)
        ROS_ERROR("tcsetattr ICANON");
 
    if(rv == -1)
        ROS_ERROR("select");
    else if(rv == 0);
        //ROS_INFO("no_key_pressed");
    else
        read(filedesc, &buff, len );
 
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(filedesc, TCSADRAIN, &old) < 0)
        ROS_ERROR ("tcsetattr ~ICANON");
    return (buff);
}
 

void print_Menu(char c, const ros::Publisher pub)
{
	switch(state)
	{
		case NONE:
			ROS_INFO("------------ Platform Test Interface Program ------------ ");
			ROS_INFO("  1.Enable Both, 2.Enable EPS, 3.Enable ACC, 4.Control ");
			ROS_INFO("--------------------------------------------------------- ");
			state = WAIT;
			break;
		case WAIT:
			if(c != NULL)
				select_state(c);
			break;
		case BOTH_ON:
			msg.stamp = ros::Time::now();
			msg.mode = 1;
			pub.publish(msg);
			state = NONE;
			break;
		case EPS_ON:
			msg.stamp = ros::Time::now();
			msg.mode = 2;
			pub.publish(msg);
			state = NONE;
			break;
		case ACC_ON:
			msg.stamp = ros::Time::now();
			msg.mode = 3;
			pub.publish(msg);
			state = NONE;
			break;
		case CONTROL:
			msg.stamp = ros::Time::now();
			msg.mode = 4;
			pub.publish(msg);
			state = CTRL_WASD;
			break;
		case CTRL_WASD:
			if(c != NULL)
				CTRL_WASD_switch(c, pub);
			break;
	}
	
}

void CTRL_WASD_switch(char c, const ros::Publisher pub)
{
	switch(c)
	{
		case 'w':
			msg.stamp = ros::Time::now();
			msg.mode = 5;				// Speed Up
			pub.publish(msg);
			break;
		case 'W':
			msg.stamp = ros::Time::now();
			msg.mode = 5;				// Speed Up
			pub.publish(msg);
			break;
		case 's':
			msg.stamp = ros::Time::now();
			msg.mode = 6;				// Speed Down
			pub.publish(msg);
			break;
		case 'S':
			msg.stamp = ros::Time::now();
			msg.mode = 6;				// Speed Down
			pub.publish(msg);
			break;
		case 'a':
			msg.stamp = ros::Time::now();
			msg.mode = 7;				// Steering Left
			pub.publish(msg);
			break;
		case 'A':
			msg.stamp = ros::Time::now();
			msg.mode = 7;				// Steering Left
			pub.publish(msg);
			break;
		case 'd':
			msg.stamp = ros::Time::now();
			msg.mode = 8;				// Steering Right
			pub.publish(msg);
			break;
		case 'D':
			msg.stamp = ros::Time::now();
			msg.mode = 8;				// Steering Right
			pub.publish(msg);
			break;

		case 'p':
			msg.stamp = ros::Time::now();
			msg.mode = 9;				// End of Control
			pub.publish(msg);
			state = NONE;
			break;
		case 'P':
			msg.stamp = ros::Time::now();
			msg.mode = 9;				// End of Control
			pub.publish(msg);
			state = NONE;
			break;
	}
}

void select_state(char c)
{
	switch(c)
	{
		case '1':
			state = BOTH_ON;
                        //ROS_INFO("State = Both Enable");
			break;
		case '2':
			state = EPS_ON;
                        //ROS_INFO("State = EPS Enable");
			break;
		case '3':
			state = ACC_ON;
                        //ROS_INFO("State = ACC Enable");
			break;
		case '4':
			state = CONTROL;
                        //ROS_INFO("State = Control");
			break;
		default:
                        //ROS_INFO("Wrong Value");
			break;
	}
}
