/*
* Unpublished Copyright (c) 2009-2017 AutonomouStuff, LLC, All Rights Reserved.
*
* This file is part of the Kvaser ROS 1.0 driver which is released under the MIT license.
* See file LICENSE included with this software or go to https://opensource.org/licenses/MIT for full license details.
*/

#include <thread>
#include <mutex>
#include <chrono>

#include <ros/ros.h>
#include <kvaser_interface.h>
#include <can_msgs/Frame.h>

using namespace AS::CAN;

int bit_rate = 500000;
int hardware_id = 0;
int circuit_id = 0;
bool global_keep_going = true;
int bit_rate2 = 500000;
int hardware_id2 = 0;
int circuit_id2 = 0;
bool global_keep_going2 = true;
std::mutex keep_going_mut;
std::mutex keep_going_mut2;
KvaserCan can_reader, can_reader2, can_writer;
ros::Publisher can_tx_pub, can_tx_pub2;

void can_read()
{
  long id;
  uint8_t msg[8];
  unsigned int size;
  bool extended;
  unsigned long t;

  const std::chrono::milliseconds loop_pause = std::chrono::milliseconds(10);
  bool keep_going = true;

  //Set local to global value before looping.
  keep_going_mut.lock();
  keep_going = global_keep_going;
  keep_going_mut.unlock();

  return_statuses ret;

  while (keep_going)
  {
    std::chrono::system_clock::time_point next_time = std::chrono::system_clock::now();
    next_time += loop_pause;

    if (!can_reader.is_open())
    {
      ret = can_reader.open(hardware_id, circuit_id, bit_rate, false);

      if (ret != OK)
        ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error opening reader: %d - %s", ret, return_status_desc(ret).c_str());
    }
    else
    {
      while ((ret = can_reader.read(&id, msg, &size, &extended, &t)) == OK)
      {
        can_msgs::Frame can_pub_msg;
        can_pub_msg.header.frame_id = "0";
        can_pub_msg.id = id;
        can_pub_msg.dlc = size;
        std::copy(msg, msg + 8, can_pub_msg.data.begin());
        can_pub_msg.header.stamp = ros::Time::now();
        can_tx_pub.publish(can_pub_msg);
      }

      if (ret != NO_MESSAGES_RECEIVED)
        ROS_WARN_THROTTLE(0.5, "Kvaser CAN Interface - Error reading CAN message: %d - %s", ret, return_status_desc(ret).c_str());
    }

    std::this_thread::sleep_until(next_time);

    //Set local to global immediately before next loop.
    keep_going_mut.lock();
    keep_going = global_keep_going;
    keep_going_mut.unlock();
  }

  if (can_reader.is_open())
  {
    ret = can_reader.close();

    if (ret != OK)
      ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error closing reader: %d - %s", ret, return_status_desc(ret).c_str());
  }
}

void can_read2()
{
  long id2;
  uint8_t msg2[8];
  unsigned int size2;
  bool extended2;
  unsigned long t2;

  const std::chrono::milliseconds loop_pause2 = std::chrono::milliseconds(10);
  bool keep_going2 = true;

  //Set local to global value before looping.
  keep_going_mut2.lock();
  keep_going2 = global_keep_going2;
  keep_going_mut2.unlock();

  return_statuses ret2;

  while (keep_going2)
  {
    std::chrono::system_clock::time_point next_time2 = std::chrono::system_clock::now();
    next_time2 += loop_pause2;

    if (!can_reader2.is_open())
    {
      ret2 = can_reader2.open(hardware_id2, circuit_id2, bit_rate2, false);

      if (ret2 != OK)
        ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error opening reader: %d - %s", ret2, return_status_desc(ret2).c_str());
    }
    else
    {
      while ((ret2 = can_reader2.read(&id2, msg2, &size2, &extended2, &t2)) == OK)
      {
        can_msgs::Frame can_pub_msg2;
        can_pub_msg2.header.frame_id = "0";
        can_pub_msg2.id = id2;
        can_pub_msg2.dlc = size2;
        std::copy(msg2, msg2 + 8, can_pub_msg2.data.begin());
        can_pub_msg2.header.stamp = ros::Time::now();
        can_tx_pub2.publish(can_pub_msg2);
      }

      if (ret2 != NO_MESSAGES_RECEIVED)
        ROS_WARN_THROTTLE(0.5, "Kvaser CAN Interface - Error reading CAN message: %d - %s", ret2, return_status_desc(ret2).c_str());
    }

    std::this_thread::sleep_until(next_time2);

    //Set local to global immediately before next loop.
    keep_going_mut2.lock();
    keep_going2 = global_keep_going2;
    keep_going_mut2.unlock();
  }

  if (can_reader2.is_open())
  {
    ret2 = can_reader2.close();

    if (ret2 != OK)
      ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error closing reader: %d - %s", ret2, return_status_desc(ret2).c_str());
  }
}


void can_rx_callback(const can_msgs::Frame::ConstPtr& msg)
{
  return_statuses ret;

  if (!can_writer.is_open())
  {
    // Open the channel.
    ret = can_writer.open(hardware_id, circuit_id, bit_rate, false);

    if (ret != OK)
    {
      ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error opening writer: %d - %s", ret, return_status_desc(ret).c_str());
    }
  }

  if (can_writer.is_open())
  {
    ret = can_writer.write(msg->id, const_cast<unsigned char*>(&(msg->data[0])), msg->dlc, msg->is_extended);

    if (ret != OK)
      ROS_WARN_THROTTLE(0.5, "Kvaser CAN Interface - CAN send error: %d - %s", ret, return_status_desc(ret).c_str());
  }
}

int main(int argc, char** argv)
{
  bool exit = false;

  // ROS initialization
  ros::init(argc, argv, "kvaser_can_bridge");
  ros::NodeHandle n;
  ros::NodeHandle priv("~");
  ros::AsyncSpinner spinner(3);

  can_tx_pub = n.advertise<can_msgs::Frame>("can_tx", 500);
  can_tx_pub2 = n.advertise<can_msgs::Frame>("can_tx2", 500);

  ros::Subscriber can_rx_sub = n.subscribe("can_rx", 500, can_rx_callback);

  // Wait for time to be valid
  while (ros::Time::now().nsec == 0);

  // CH1

  if (priv.getParam("can_hardware_id", hardware_id))
  {
    ROS_INFO("Kvaser CAN Interface - Got hardware_id: %d", hardware_id);

    if (hardware_id <= 0)
    {
      ROS_ERROR("Kvaser CAN Interface - CAN hardware ID is invalid.");
      exit = true;
    }
  }

  if (priv.getParam("can_circuit_id", circuit_id))
  {
    ROS_INFO("Kvaser CAN Interface - Got can_circuit_id: %d", circuit_id);

    if (circuit_id < 0)
    {
      ROS_ERROR("Kvaser CAN Interface - Circuit ID is invalid.");
      exit = true;
    }
  }

  if (priv.getParam("can_bit_rate", bit_rate))
  {
    ROS_INFO("Kvaser CAN Interface - Got bit_rate: %d", bit_rate);
    
    if (bit_rate < 0)
    {
      ROS_ERROR("Kvaser CAN Interface - Bit Rate is invalid.");
      exit = true;
    }
  }
	
  // CH2
  if (priv.getParam("can_hardware_id2", hardware_id2))
  {
    ROS_INFO("Kvaser CAN Interface - Got hardware_id: %d", hardware_id2);

    if (hardware_id2 <= 0)
    {
      ROS_ERROR("Kvaser CAN Interface - CAN hardware ID is invalid.");
      exit = true;
    }
  }

  if (priv.getParam("can_circuit_id2", circuit_id2))
  {
    ROS_INFO("Kvaser CAN Interface - Got can_circuit_id: %d", circuit_id2);

    if (circuit_id2 < 0)
    {
      ROS_ERROR("Kvaser CAN Interface - Circuit ID is invalid.");
      exit = true;
    }
  }

  if (priv.getParam("can_bit_rate2", bit_rate2))
  {
    ROS_INFO("Kvaser CAN Interface - Got bit_rate: %d", bit_rate2);
    
    if (bit_rate2 < 0)
    {
      ROS_ERROR("Kvaser CAN Interface - Bit Rate is invalid.");
      exit = true;
    }
  }


  if (exit)
    return 0;

  // Start CAN receiving thread.
  std::thread can_read_thread(can_read);
  std::thread can_read_thread2(can_read2);
  spinner.start();

  ros::waitForShutdown();

  return_statuses ret = can_writer.close();
  return_statuses ret2 = can_writer.close();

  if (ret != OK)
    ROS_ERROR("Kvaser CAN Interface - Error closing writer: %d - %s", ret, return_status_desc(ret).c_str());

  keep_going_mut.lock();
  global_keep_going = false;
  keep_going_mut.unlock();

  can_read_thread.join();

  if (ret2 != OK)
    ROS_ERROR("Kvaser CAN Interface - Error closing writer: %d - %s", ret2, return_status_desc(ret2).c_str());

  keep_going_mut2.lock();
  global_keep_going2 = false;
  keep_going_mut2.unlock();

  can_read_thread2.join();

  return 0;
}
