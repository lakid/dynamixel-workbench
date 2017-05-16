/*******************************************************************************
* Copyright (c) 2016, ROBOTIS CO., LTD.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of ROBOTIS nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* Author: Taehoon Lim (Darby) */

#ifndef DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H
#define DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ros/ros.h>
#include <dynamixel_workbench_toolbox/dynamixel_tool.h>
#include <dynamixel_workbench_msgs/MotorStateList.h>
#include <dynamixel_workbench_msgs/SetJointPosition.h>

#include <dynamixel_sdk/dynamixel_sdk.h>

#define PAN_TILT_MOTOR 0
#define JOINT1_MOTOR      0
#define JOINT1_2_MOTOR     1
#define JOINT1_3_MOTOR     2
#define JOINT2_MOTOR     3

namespace dynamixel_workbench_position_control
{
class DynamixelWorkbenchPositionControl
{
 public:
  dynamixel::PortHandler *portHandler_;
  dynamixel::PacketHandler *packetHandler_;

 private:
  // ROS NodeHandle
  ros::NodeHandle nh_;
  ros::NodeHandle nh_priv_;
  // ROS Parameters
  bool is_debug_;
  // ROS Topic Publisher
  ros::Publisher dynamixel_state_pub_;
  // ROS Service Server
  ros::ServiceServer position_control_server;
  // Parameters
  std::vector<dynamixel_tool::DynamixelTool *> dynamixel_;

  std::string device_name_;
  std::string motor_model_;
  int motor_id_;
  float protocol_version_;
  int baud_rate_;
  int profile_velocity_;
  int profile_acceleration_;

  std::map<std::string, std::vector<int64_t> *> read_data_;
 public:
  DynamixelWorkbenchPositionControl();
  ~DynamixelWorkbenchPositionControl();
  bool dynamixelControlLoop(void);

 private:
  bool initDynamixelWorkbenchPositionControl(void);
  bool shutdownDynamixelWorkbenchPositionControl(void);

  bool initMotor(std::string motor_model, uint8_t motor_id, float protocol_version);

  bool readDynamixelRegister(uint8_t id, uint16_t addr, uint8_t length, int64_t *value);
  bool readMotorState(std::string addr_name);

  bool syncWriteDynamixels(uint16_t addr, uint8_t length, std::vector<int64_t> motor_values);
  bool writeTorque(bool onoff);
  bool writeProfile(int velocity, int acceleration);
  bool writePosition(std::vector<int64_t> pos);

  int64_t convertRadian2Value(double radian);

  bool getPublishedMsg(void);
  bool controlPanTiltMotorCallback(dynamixel_workbench_msgs::SetJointPosition::Request &req,
                                   dynamixel_workbench_msgs::SetJointPosition::Response &res);
};
}

#endif //DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H
