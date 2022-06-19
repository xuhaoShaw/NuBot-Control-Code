/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include<tf/transform_broadcaster.h>
#include <std_msgs/Empty.h>
#include<geometry_msgs/Twist.h>

ros::NodeHandle  nh;


void calculate_vel(const geometry_msgs::Twist& cmd_vel)
{
  digitalWrite(13, HIGH); 
  delay(100);
  digitalWrite(13, LOW); 
}

ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", &calculate_vel);


void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(500);
}
