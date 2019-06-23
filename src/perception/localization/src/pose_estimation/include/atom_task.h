#pragma once
#include <ros/ros.h>
#include "std_msgs/Float64.h"

class AtomTask{
  // ========================
  // functions
  protected:
    //AtomTask(){};
    AtomTask(std::string task_name, double period);
    ~AtomTask();

  public:
    void Exec();

  protected:
    virtual void Init()=0;
    virtual void Run()=0;
    virtual void Publish()=0;
    virtual void Terminate()=0;

  private:
    ros::Publisher pub_life_;
    void PublishLifeSignal();

  protected:
    std::string	  task_name_;
    double	  task_period_;   
    int		  task_rate_;
    ros::Time	  update_time_;
    ros::Duration duration_time_;
    ros::Duration execution_time_;
};

AtomTask::AtomTask(std::string task_name, double period)
:task_name_(""),
task_period_(0.0),
task_rate_(0.0)
{
  task_name_ = task_name;
  task_period_ = period;        
  task_rate_ = (int)(1/period);

  update_time_ = ros::Time::now();
  duration_time_ = ros::Duration(0.0);
  execution_time_ = ros::Duration(0.0);

  ros::NodeHandle nh;
  pub_life_ = nh.advertise<std_msgs::Float64>("task_"+task_name_, 100);
}

AtomTask::~AtomTask(){
}

void AtomTask::Exec(){
  // Init
  Init();
  // Loop
  ros::Rate loop_rate(task_rate_);
  //ROS_INFO( "rate: %d", task_rate_ );
  while( ros::ok() ){
    ros::Time time_now = ros::Time::now();
    duration_time_ = time_now - update_time_;
    update_time_ = time_now;
    
    Run();

    execution_time_ = ros::Time::now() - update_time_;

    //ROS_INFO("ROS_TIME : %.9f",ros::Time::now().toSec());  //XXXXXXXX
    //ROS_INFO("atom_task : %.9f",execution_time_.toSec());  //XXXXXXXX

    PublishLifeSignal();
    Publish();    

    ros::spinOnce();
    loop_rate.sleep();
  }
  // Terminate
  Terminate();
}

void AtomTask::PublishLifeSignal(){
  std_msgs::Float64 time;
  time.data = update_time_.toSec();
  pub_life_.publish( time  );
}
