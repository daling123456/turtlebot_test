#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "tf/transform_datatypes.h"
std::string robot_name, vir;

void poseCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
	// 创建tf的广播器
	static tf::TransformBroadcaster br;
	static tf::TransformBroadcaster br1;

	// 初始化tf数据
	tf::Transform transform;
	tf::Transform transform1;
	transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
	transform1.setOrigin( tf::Vector3(2, 0, 0.0) );
    
	double roll, pitch, yaw;
	tf::Quaternion q;
	tf::Quaternion quat;
	tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
  	tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
	q.setRPY(0.0, 0.0, yaw);
	transform.setRotation(q);
	transform1.setRotation(tf::Quaternion(0, 0, 0, 1));

	// 广播world与海龟坐标系之间的tf数据
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", robot_name));
	br1.sendTransform(tf::StampedTransform(transform1, ros::Time::now(), robot_name, vir));
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,robot_name+"my_tf_brocaster");
    if (argc!=3)
    {
        ROS_ERROR("输入参数数目错误");
    }
    else
    {
        robot_name=argv[1];
        vir=argv[2];
    }
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe(robot_name+"/base_pose_ground_truth", 10, &poseCallback);
    ros::spin();
    return 0;
}
