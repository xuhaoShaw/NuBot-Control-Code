#include<ros.h>
#include<ros/time.h>
#include<tf/transform_broadcaster.h>
#include<geometry_msgs/Twist.h>
#include <Wire.h>
#include <MsTimer2.h> //中断库

float wheelDist = 0.1;
float wheelRadius = 0.044;
int left_vel = 0;
int right_vel = 0;

int pinA=2; // 定义2号端口为A脉冲输入端口
int pinB=3;// 定义3号端口为B脉冲输入端口
int pwm1=9;//pwm1输出口
int ppsA=0; //记录A脉冲的次数
int pusNum=350;
int velocity=0; //速度

int kp=6;
int ki=2;
int pwm_num=0;
int bias=0;
int last_bias=0;

ros::NodeHandle nh;

void calculate_vel(const geometry_msgs::Twist& cmd_vel)
{
  digitalWrite(13, HIGH-digitalRead(13)); 
  float v = cmd_vel.linear.x;
  float w = cmd_vel.angular.z;
  left_vel = 100*(v-w*wheelDist/2);//danwei cm/s
  right_vel = 100*(v+w*wheelDist/2);
}

void flash()
{
  velocity=v(ppsA); //计算速度,单位cm/s
  Serial.print(velocity);
  vel_pid(velocity,right_vel);
  analogWrite(pwm1,pwm_num);
  ppsA=0;    //脉冲A计数归0
}

void CountA() {
  ppsA++;
} 

void vel_pid(int velocity,int cmd_vel)//计算pwm输出
{
  bias=cmd_vel-velocity;
  pwm_num+=kp*(bias-last_bias)+ki*bias;
  last_bias=bias;
  if(pwm_num>255){pwm_num=255;}
  else if(pwm_num<120){pwm_num=0;}
}

int v(int n) //转速计算函数
{
  int vel =135*n/pusNum; // n/(pusNum*0.1)
  return vel;
}

geometry_msgs::Twist current_vel;
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &calculate_vel);
ros::Publisher pub_vel("pub_vel", &current_vel);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(pwm1, OUTPUT);
  attachInterrupt(0,CountA, FALLING);
  MsTimer2::set(100, flash);       // 中断设置函数，0.1s
  MsTimer2::start();   //开始计时
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub_vel);
}

void loop() {
  // put your main code here, to run repeatedly:
  current_vel.linear.x=velocity;
  pub_vel.publish(&current_vel);
  nh.spinOnce();
  delay(100);
}
