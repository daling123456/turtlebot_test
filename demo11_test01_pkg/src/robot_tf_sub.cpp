#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
//#include "sensor_msgs/LaserScan.h"
std::string r1,r2;
int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "my_tf_listener");

    // 创建节点句柄
	ros::NodeHandle node;
	
	if(argc!=3)
	{
		ROS_ERROR("input error!!!!!!!!");
	}else{
		r1=argv[1];
		r2=argv[2];
	}

	// 创建发布tb3_1速度控制指令的发布者
	ros::Publisher tb3_1_vel = node.advertise<geometry_msgs::Twist>(r2+"/cmd_vel", 10);

	// 创建tf的监听器
	tf::TransformListener listener;

	ros::Rate rate(10.0);
	while (node.ok())
	{
		// 获取turtle1与turtle2坐标系之间的tf数据
		tf::StampedTransform transformfl;
		tf::StampedTransform transformlf;
		try
		{
		        listener.waitForTransform(r2, r1, ros::Time(0), ros::Duration(3.0));
			listener.lookupTransform(r2, r1, ros::Time(0), transformfl);
			
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}
		// try
		// {
		//         listener.waitForTransform(r1, r2, ros::Time(0), ros::Duration(3.0));
		// 	listener.lookupTransform(r1, r2, ros::Time(0), transformlf);
			
		// }
		// catch (tf::TransformException &ex) 
		// {
		// 	ROS_ERROR("%s",ex.what());
		// 	ros::Duration(1.0).sleep();
		// 	continue;
		// }

		// 根据tb3_0与tb3_1坐标系之间的位置关系，发布turtle2的速度控制指令
		geometry_msgs::Twist vel_msg;
		vel_msg.angular.z = 2.0 * atan2(transformfl.getOrigin().y(),
				                        transformfl.getOrigin().x());
		vel_msg.linear.x = 0.2 * sqrt(pow(transformfl.getOrigin().x(), 2) +
				                      pow(transformfl.getOrigin().y(), 2));
		tb3_1_vel.publish(vel_msg);

		rate.sleep();
		ros::spinOnce();
	}
	return 0;
};
