#include<ros/ros.h>
#include<tf/transform_broadcaster.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_tf_publish");
    ros::NodeHandle n;

    ros::Rate r(10);
    tf::TransformBroadcaster broadcaster_laser;
    tf::TransformBroadcaster broadcaster_IMU;
     while(n.ok())
     {
         broadcaster_laser.sendTransform(
                 tf::StampedTransform(
                     tf::Transform(tf::Quaternion(0,0,0,1),tf::Vector3(0,0,0.1)),
                    ros::Time::now(),"base_link","laser"));
        broadcaster_IMU.sendTransform(
                 tf::StampedTransform(
                     tf::Transform(tf::Quaternion(0,0,0,1),tf::Vector3(0.05,0,0.1)),
                    ros::Time::now(),"base_link","IMU"));
         r.sleep();
     }
}
