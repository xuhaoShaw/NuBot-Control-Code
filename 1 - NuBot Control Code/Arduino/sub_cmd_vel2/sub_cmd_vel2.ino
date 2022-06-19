#include<ros.h>
#include<ros/time.h>
#include<tf/transform_broadcaster.h>
#include<geometry_msgs/Twist.h>
#include <Wire.h>
#include <MsTimer2.h> //中断库

float wheelDist = 0.112;
float wheelRadius = 0.044;
int left_vel = 0;
int right_vel = 0;

int pinA1=2; // 定义2号端口为A脉冲输入端口，右边是B，左边是A
int pinA2=4;
int pinB1=3;// 定义3号端口为B脉冲输入端口
int pinB2=5;
int IN1=6;//L298N的IN1-IN4输入
int IN2=7;
int IN3=8;
int IN4=11;
int pwm_A=9;//pwm_A输出口
int pwm_B=10;//pwm_B输出口
int ppsA=0; //记录A脉冲的次数
int ppsB=0;//记录B脉冲的次数
int pusNum=693;
int velocity_A=0; //速度
int velocity_B=0; //速度
int real_velocity_A=0; //速度
int real_velocity_B=0; //速度
int pwm_num_A=0;//输出的pwm数
int pwm_num_B=0;
char mode=1;//车的状态，1_前进;2_后退;3_左转;4_右转;
char mode_A;//A轮转向状态，1_前进，0_后退
char mode_B;

int kp=6;
int ki=2;
int bias=0;
int last_bias_A=0;
int last_bias_B=0;

ros::NodeHandle nh;

void calculate_vel(const geometry_msgs::Twist& cmd_vel)
{
  digitalWrite(13, HIGH-digitalRead(13)); 
  float v = cmd_vel.linear.x;
  float w = cmd_vel.angular.z;
  left_vel = 100*(v-w*wheelDist/2);//danwei cm/s
  right_vel = 100*(v+w*wheelDist/2);
  if(left_vel>0)
    {
      if(right_vel>0){mode=1;}
      else{mode=4;}
    }
   else
   {
      if(right_vel>0){mode=3;}
      else{mode=2;}
   }
  
}

void flash()
{
  velocity_A=v(ppsA); //计算速度,单位cm/s
  velocity_B=v(ppsB); //计算速度,单位cm/s
  //包含转向的左右轮速度
  if(mode_A==1){real_velocity_A=velocity_A;}
  else{real_velocity_A=-velocity_A;}
  if(mode_B==1){real_velocity_B=velocity_B;}
  else{real_velocity_B=-velocity_B;}
  
  pwm_num_A=vel_pid(velocity_A,right_vel,pwm_num_A,last_bias_A);
  pwm_num_B=vel_pid(velocity_B,left_vel,pwm_num_B,last_bias_B);
  switch(mode)
  {
    case 1:{
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
      digitalWrite(IN3,LOW);
      digitalWrite(IN4,HIGH);
      break;}
    case 2:{
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,LOW);
      break;}
    case 3:{
      digitalWrite(IN1,HIGH);
      digitalWrite(IN2,LOW);
      digitalWrite(IN3,LOW);
      digitalWrite(IN4,HIGH);
      break;}
    case 4:{
      digitalWrite(IN1,LOW);
      digitalWrite(IN2,HIGH);
      digitalWrite(IN3,HIGH);
      digitalWrite(IN4,LOW);
      break;}
    default:{
      break;}
  }
  analogWrite(pwm_A,pwm_num_A);
  analogWrite(pwm_B,pwm_num_B);
  ppsA=0;    //脉冲A计数归0
  ppsB=0;    //脉冲A计数归0
}

void CountA() {
  if(digitalRead(pinA2) == HIGH) //B脉冲为高电平
  {mode_A=0;}
  if(digitalRead(pinA2) == LOW) //B脉冲为高电平
  {mode_A=1;}
  ppsA++;
} 

void CountB() {
  if(digitalRead(pinB2) == HIGH) //B脉冲为高电平
  {mode_B=1;}
  if(digitalRead(pinB2) == LOW) //B脉冲为高电平
  {mode_B=0;}
  ppsB++;
} 

int vel_pid(int velocity,int cmd_vel,int pwm_num,int last_bias)//计算pwm输出
{
  cmd_vel=abs(cmd_vel);
  bias=cmd_vel-velocity;
  pwm_num+=kp*(bias-last_bias)+ki*bias;
  last_bias=bias;
  if(pwm_num>255){pwm_num=255;}
  else if(pwm_num<50){pwm_num=0;}
  return pwm_num;
}

int v(int n) //速度计算函数
{
  int vel =138*n/pusNum; // n/(pusNum*0.1)
  return vel;
}

geometry_msgs::Twist current_vel;
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", &calculate_vel);
ros::Publisher pub_vel("pub_vel", &current_vel);

void setup() {
  pinMode(13, OUTPUT);
  pinMode(pwm_A, OUTPUT);
  pinMode(pwm_B, OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  attachInterrupt(0,CountA, FALLING);
  attachInterrupt(1,CountB, FALLING);
  MsTimer2::set(100, flash);       // 中断设置函数，0.1s
  MsTimer2::start();   //开始计时
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(pub_vel);
}

void loop() {
  // put your main code here, to run repeatedly:
  current_vel.linear.x=real_velocity_A;
  current_vel.linear.y=real_velocity_B;
  current_vel.angular.x=pwm_num_A;
  current_vel.angular.y=pwm_num_B;
  pub_vel.publish(&current_vel);
  nh.spinOnce();
  delay(100);
}
