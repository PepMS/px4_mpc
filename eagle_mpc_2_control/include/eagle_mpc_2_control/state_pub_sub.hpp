#ifndef EAGLE_MPC_2_CONTROL_STATE_PUB_SUB_HPP
#define EAGLE_MPC_2_CONTROL_STATE_PUB_SUB_HPP

#include <chrono>
#include <vector>

#include <rclcpp/rclcpp.hpp>

#include <Eigen/Dense>

#include <px4_msgs/msg/timesync.hpp>
#include <px4_msgs/msg/vehicle_local_position.hpp>
#include <px4_msgs/msg/vehicle_attitude.hpp>
#include <px4_msgs/msg/vehicle_angular_velocity.hpp>

#include <px4_ros_com/frame_transforms.h>

#include "eagle_mpc_2_msgs/msg/platform_state.hpp"

// Transformation tools
const Eigen::Quaterniond FRD_FLU_Q =
    px4_ros_com::frame_transforms::utils::quaternion::quaternion_from_euler(M_PI, 0.0, 0.0);
const Eigen::Quaterniond NWU_NED_Q =
    px4_ros_com::frame_transforms::utils::quaternion::quaternion_from_euler(M_PI, 0.0, 0.0);

class StatePubSub {
 public:
  explicit StatePubSub(rclcpp::Node::SharedPtr node, const bool& pub = true);
  virtual ~StatePubSub();

 protected:
  rclcpp::Node::SharedPtr node_;

  std::atomic<uint64_t> timestamp_;  //!< common synced timestamped

  // Class variables
  Eigen::VectorXd state_;  // local pos in inertial frame (NWU), quaternion (x,y,z,w. From FLU to NWU), lin. velocity
                           // and ang. velocity (FLU base frame)
  Eigen::Vector3d vel_ned_;
  Eigen::Vector3d vel_frd_;
  Eigen::Quaterniond q_ned_frd_;
  Eigen::Quaterniond q_nwu_flu_;

 private:
  // pubs
  rclcpp::Publisher<eagle_mpc_2_msgs::msg::PlatformState>::SharedPtr platform_state_publisher_;

  // subs
  rclcpp::Subscription<px4_msgs::msg::Timesync>::SharedPtr timesync_sub_;
  rclcpp::Subscription<px4_msgs::msg::VehicleLocalPosition>::SharedPtr local_position_subs_;
  rclcpp::Subscription<px4_msgs::msg::VehicleAttitude>::SharedPtr attitude_subs_;
  rclcpp::Subscription<px4_msgs::msg::VehicleAngularVelocity>::SharedPtr angular_velocity_subs_;

  // State subscribers callbacks
  void timeSyncCallback(const px4_msgs::msg::Timesync::UniquePtr msg);
  void vehicleLocalPositionCallback(const px4_msgs::msg::VehicleLocalPosition::UniquePtr msg);
  void vehicleAttitudeCallback(const px4_msgs::msg::VehicleAttitude::UniquePtr msg);
  void vehicleAngularVelocityCallback(const px4_msgs::msg::VehicleAngularVelocity::UniquePtr msg);

  // methods
  void publish_platform_state();

  // msgs
  eagle_mpc_2_msgs::msg::PlatformState platform_state_msg_;

  bool pub_enabled_;
};
#endif
