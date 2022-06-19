#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

double wheelDist=0.112;
double dt=0.1;
double x=0;
double y=0;
double th=0;
double v=0;
double w=0;
geometry_msgs::TransformStamped odom_trans;
ros::Time current_time;

void odomCallback(const geometry_msgs::Twist& pub_vel)
{
	v= (pub_vel.linear.x+pub_vel.linear.y)/200;
	w= (pub_vel.linear.y-pub_vel.linear.x)/100/wheelDist;
	double dth = w*dt;
	th += dth;
	double dx = v*dt*cos(th);
	double dy = v*dt*sin(th);
	x += dx;
	y += dy;
	current_time = ros::Time::now();
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "odom_publish");
	ros::NodeHandle n;
    ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom",10);
	ros::Subscriber sub = n.subscribe("/pub_vel", 10, odomCallback);
	tf::TransformBroadcaster broadcaster;
	ros::Rate loop_rate(10);

	while (ros::ok()) {
	ros::spinOnce();
	odom_trans.header.frame_id = "odom";
	odom_trans.child_frame_id = "base_link";
	current_time = ros::Time::now();
	odom_trans.header.stamp = current_time;
	odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
	ROS_INFO("[%f]",x);
    ROS_INFO("[%f]",y);
    ROS_INFO("[%f]",th);
	odom_trans.transform.translation.z = 0.0;
	odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(th);
	// publishing the odometry and the new tf
	broadcaster.sendTransform(odom_trans);
    ROS_INFO("I published odom_tf ");

    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    odom.child_frame_id = "base_link";
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(th);
    odom.twist.twist.linear.x = v;
    odom.twist.twist.angular.z = w;
    odom_pub.publish(odom);
    ROS_INFO("I published odom");
	loop_rate.sleep();
	}
	return 0;
}
