/***************************************************************************
* Copyright (c) 2013-2017, Rethink Robotics Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**************************************************************************/
#ifndef _SAWYER_GAZEBO___ARM_CONTROLLER_INTERFACE_H_
#define _SAWYER_GAZEBO___ARM_CONTROLLER_INTERFACE_H_

#include <mutex>

#include <ros/ros.h>
#include <controller_manager/controller_manager.h>
#include <realtime_tools/realtime_box.h>

#include <std_msgs/Float64.h>
#include <intera_core_msgs/JointCommand.h>
#include <intera_core_msgs/SEAJointState.h>


namespace sawyer_gazebo {
  class ArmControllerInterface
  {
  public:
    void init(ros::NodeHandle& nh, std::string side,
         boost::shared_ptr<controller_manager::ControllerManager> controller_manager);

  private:
    // mutex for re-entrant calls to modeCommandCallback
    std::mutex mtx_;
    int current_mode_;
    bool usr_command_;
    intera_core_msgs::JointCommand last_jnt_cmd_;
    std::string side_;
    ros::Timer update_timer_;
    ros::Time last_command_time_;
    ros::Subscriber joint_command_timeout_sub_;
    ros::Subscriber joint_command_sub_;
    ros::Subscriber joint_state_sub_;
    ros::Publisher jnt_cmd_pub_;
    boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
    realtime_tools::RealtimeBox< std::shared_ptr<const intera_core_msgs::SEAJointState> > joint_state_buffer_;
    realtime_tools::RealtimeBox< std::shared_ptr<const ros::Duration> > command_timeout_buffer_;

  protected:
    /* Callback to capture and store the current joint states of the robot
     */
    void jointStateCallback(const intera_core_msgs::SEAJointStateConstPtr& msg);
    void jointCommandTimeoutCallback(const std_msgs::Float64 msg);
    void jointCommandCallback(const intera_core_msgs::JointCommandConstPtr& msg);
    std::string getControllerString(std::string mode_str);
    void publishCmdCurrentPose(bool usr_cmd);
    void update(const ros::TimerEvent& e);

  };
}
#endif // #ifndef __SAWYER_GAZEBO__ARM_CONTROLLER_INTERFACE_H_
