#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/UInt8.h>

#define OMEGA_GRIP_MAX 0.495
#define GRIPPER_POSITION_MAX 102

class TeleoperationController
{
private:
  ros::Publisher gripper_command_pub_;
  ros::Subscriber omega_position_sub_;
  ros::NodeHandle* nh_;
  std_msgs::UInt8 gripper_command_msg;

public:
  TeleoperationController(ros::NodeHandle* node_handle) : nh_(node_handle) 
  {
    gripper_command_pub_ = nh_->advertise<std_msgs::UInt8>("gripper_in_position", 1);
    omega_position_sub_ = nh_->subscribe("omega_out_grip_angle", 1, &TeleoperationController::positionCallback, this);
  }

  void positionCallback(const std_msgs::Float64::ConstPtr& msg)
  {
    double omega_grip_gap = msg->data;
    int gripper_position_desired = GRIPPER_POSITION_MAX - omega_grip_gap/OMEGA_GRIP_MAX * GRIPPER_POSITION_MAX;
    printf("%d\n", gripper_position_desired);
    gripper_command_msg.data = gripper_position_desired;
    gripper_command_pub_.publish(gripper_command_msg);
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "teleoperation_controller");
  ros::NodeHandle nh;
  ros::AsyncSpinner spinner(1);
  spinner.start();

  TeleoperationController teleoperation_controller(&nh);

  ros::Rate loop_rate(100);

  while (ros::ok()) 
  {
    loop_rate.sleep();
  }

}
