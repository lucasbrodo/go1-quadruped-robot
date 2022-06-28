﻿// The MIT License

// Copyright (c) 2022
// Robot Motion and Vision Laboratory at East China Normal University
// Contact:tophill.robotics@gmail.com

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef QR_ROBOT_CONFIG_H
#define QR_ROBOT_CONFIG_H

#include <iostream>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>

/**
 * @brief a class that will load all robot configs from YAML file
 */
class qrRobotConfig
{
public:

  friend class qrRobot;
  friend class qrRobotState;
  /**
   * @brief Construction of qrRobotConfig
   */
  qrRobotConfig();

  /**
   * @brief Construction of qrRobotConfig
   * @param path: the path to the YAML config file
   */
  qrRobotConfig(std::string path);

  /**
   * @brief Destruction of qrRobotConfig
   */
  ~qrRobotConfig();

  /**
   * @brief load parameter of the robot
   * @param path: the path to the YAML config file
   */
  void Load(std::string path);

  /**
   * @brief calculate Jacobian of a leg with leg ID and angles
   * @param q: angles
   * @param legId: which leg to calculate
   * @return
   */
  Eigen::Matrix<float, 3, 3> AnalyticalLegJacobian(Eigen::Matrix<float, 3, 1> &q, int legId);

  /**
   * @brief calculate foot position in base frame of robot
   * @param q: joint angles
   * @return foot position in base frame
   */
  Eigen::Matrix<float, 3, 4> JointAngles2FootPositionInBaseFrame(Eigen::Matrix<float, 12, 1> q);

  /**
   * @brief convert foot position to joint angles
   * @param legId: which leg to calculate
   * @param position: the position of the foothold
   * @return position(x, y, z)
   */
  Eigen::Matrix<float, 3, 1> FootPosition2JointAngles(Eigen::Matrix<float, 3, 1> position, int legId);


  /**
   * @brief convert foot velocity2JointVelocity
   * @param q: joint angles
   * @param v: fothold velocity
   * @param legId: which leg to convert
   * @return joint velocity of the leg
   */
  Eigen::Matrix<float, 3, 1> FootVelocity2JointVelocity(
      Eigen::Matrix<float, 3, 1> q, Eigen::Matrix<float, 3, 1> v, int legId);

  /**
   * @brief convert foot velocity2JointVelocity
   * @param q: joint angles
   * @param v: fothold velocity
   * @param legId: which leg to convert
   * @return joint velocity of the leg
   */
  Eigen::Matrix<float, 3, 1> JointVelocity2FootVelocity(
      Eigen::Matrix<float, 3, 1> q, Eigen::Matrix<float, 3, 1> v, int legId);

  /**
   * @brief get hip positions
   * @return hip positions of 4 legs
   */
  inline Eigen::Matrix<float, 3, 4> GetHipPositionsInBaseFrame(){
    return defaultHipPosition;
  }

  inline Eigen::Matrix<float, 12, 1> GetKps() const{
      return motorKps;
  }

  inline Eigen::Matrix<float, 12, 1> GetKds() const{
      return motorKds;
  }

  static const unsigned int numMotor  = 12;

  /**
   * @brief number of legs
   */
  static const unsigned int numLegs   = 4;

  /**
   * @brief DOF of each leg
   */
  static const unsigned int dofPerLeg = 3;

  // TODO: explain
  static constexpr float footHoldOffset = 0.1f;

  /**
   * @brief control mode of the robot
   */
  int controlMode;


  bool isSim;


  inline Eigen::Matrix<float, 3, 3> GetBodyInertia() const{
    return bodyInertia;
  }

  inline float GetBodyMass() const{
    return bodyMass;
  }

  inline void SetYawOffset(float yawOffset){
    this->yawOffset = yawOffset;
  }

private:

  /**
   * @brief the weight of the main body
   */
  float bodyMass;

  /**
   * @brief the height of the main body
   */
  float bodyHeight;

  /**
   * @brief the length of three links, including hip, upper link and lowerlink
   */
  float hipLength, upperLength, lowerLength;

  // check difference of these two;
  Eigen::Matrix<float, 3, 4> defaultHipPosition;

  /**
   * @brief offset of hip
   */
  Eigen::Matrix<float, 3, 4> hipOffset;

  /**
   * @brief offset of center of mass
   */
  Eigen::Matrix<float, 3, 1> comOffset;

  /**
   * @brief the tensor of inertia of the body
   */
  Eigen::Matrix<float, 3, 3> bodyInertia;

  /**
   * @brief motor position stiffness (unit: N.m/rad )
   */
  Eigen::Matrix<float, 12, 1> motorKps;


  /**
   * @brief velocity stiffness (unit: N.m/(rad/s) )
   */
  Eigen::Matrix<float, 12, 1> motorKds;

  // TODO: check these variables
  // Eigen::Matrix<float, 12, 1> jointDirection; this is not used
  // Eigen::Matrix<float, 12, 1> jointOffset; this is not used
  // Eigen::Matrix<float, 12, 1> standUpMotorAngles; should not used here
  // Eigen::Matrix<float, 12, 1> sitDownMotorAngles; should not used here

  float yawOffset = 0.f;

  /**
   * @brief load kps parameter from YAML file
   * @param node: node that load YAML file
   */
  void LoadKps(YAML::Node &node);

  /**
   * @brief load kps parameter from YAML file
   * @param path: file path
   */
  void LoadKds(YAML::Node &node);


  void LoadComOffset(YAML::Node &node);

  void LoadHipOffset(YAML::Node &node);

  void LoadHipPosition(YAML::Node &node);
  /**
   * @brief convert foot position in hip frame to joint angles
   * @param footPosition: the position(x, y, z) of foot point
   * @param hipSign: FR & RR: 1, FL & RL: -1
   * @return joint angles
   */
  Eigen::Matrix<float, 3, 1> FootPositionInHipFrame2JointAngle(Eigen::Matrix<float, 3, 1> &footPosition, int hipSign);

  // TODO: discuss this. world frame?
  /**
   * @brief convert angles q to joint position in hip frame
   * @param q: joint angles
   * @param hipSign: FR & RR: 1, FL & RL: -1
   * @return
   */
  Eigen::Matrix<float, 3, 1> JointAngles2FootPositionInHipFrame(Eigen::Matrix<float, 3, 1> q, int hipSign);

  /**
   * @brief mapping of different control modes
   */
  static std::unordered_map<int, std::string> modeMap;


};

#endif // QR_ROBOT_CONFIG_H
