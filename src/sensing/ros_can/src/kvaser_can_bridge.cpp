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

std::mutex keep_going_MUT;
KvaserCan can_READER, can_writer;
ros::Publisher can_tx_PUB;

void can_READ()
{
  long id_;
  uint8_t msg_[8];
  unsigned int size_;
  bool extended_;
  unsigned long t_;

  const std::chrono::milliseconds loop_pause_ = std::chrono::milliseconds(10);
  bool keep_going_ = true;

  //Set local to global value before looping.
  keep_going_MUT.lock();
  keep_going_ = global_keep_going2;
  keep_going_MUT.unlock();

  return_statuses ret_;

  while (keep_going_)
  {
    std::chrono::system_clock::time_point next_time_ = std::chrono::system_clock::now();
    next_time_ += loop_pause_;

    if (!can_READER.is_open())
    {
      ret_ = can_READER.open(hardware_id2, circuit_id2, bit_rate2, false);

      if (ret_ != OK)
        ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error opening reader: %d - %s", ret_, return_status_desc(ret_).c_str());
    }
    else
    {
      while ((ret_ = can_READER.read(&id_, msg_, &size_, &extended_, &t_)) == OK)
      {
        can_msgs::Frame can_pub_MSG;
        can_pub_MSG.header.frame_id = "0";
        can_pub_MSG.id = id_;
        can_pub_MSG.dlc = size_;
        std::copy(msg_, msg_ + 8, can_pub_MSG.data.begin());
        can_pub_MSG.header.stamp = ros::Time::now();
        can_tx_PUB.publish(can_pub_MSG);
      }

      if (ret_ != NO_MESSAGES_RECEIVED)
        ROS_WARN_THROTTLE(0.5, "Kvaser CAN Interface - Error reading CAN message: %d - %s", ret_, return_status_desc(ret_).c_str());
    }

    std::this_thread::sleep_until(next_time_);

    //Set local to global immediately before next loop.
    keep_going_MUT.lock();
    keep_going_ = global_keep_going2;
    keep_going_MUT.unlock();
  }

  if (can_READER.is_open())
  {
    ret_ = can_READER.close();

    if (ret_ != OK)
      ROS_ERROR_THROTTLE(0.5, "Kvaser CAN Interface - Error closing reader: %d - %s", ret_, return_status_desc(ret_).c_str());
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

  can_tx_PUB = n.advertise<can_msgs::Frame>("can_tx", 500);

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
  std::thread can_read_thread_(can_READ);
  spinner.start();

  ros::waitForShutdown();

  return_statuses ret_ = can_writer.close();

  if (ret_ != OK)
    ROS_ERROR("Kvaser CAN Interface - Error closing writer: %d - %s", ret_, return_status_desc(ret_).c_str());

  keep_going_MUT.lock();
  global_keep_going2 = false;
  keep_going_MUT.unlock();

  can_read_thread_.join();

  return 0;
}
