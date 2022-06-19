
#include <Wire.h>
#include <MsTimer2.h> //中断库

int pinA=2; // 定义2号端口为A脉冲输入端口
int pinB=3;// 定义3号端口为B脉冲输入端口
int ppsA=0; //记录A脉冲的次数
int pusNum=350;
float m=0; //定义位移
char aen='+';
char ben='-';
char c; //方向
int velocity=0; //速度
int cmd_vel=30;//设定的速度，单位cm/s
int pwm1=9;
int kp=6;
int ki=2;
int pwm_num=0;
int bias=0;
int last_bias=0;

void CountA() {
  if(digitalRead(pinB) == HIGH) //B脉冲为高电平
  {c=aen;}
  if(digitalRead(pinB) == LOW) //B脉冲为高电平
  {c=ben;}
  ppsA++;
} 

void vel_pid(int velocity,int cmd_vel)//计算pwm输出
{
  bias=cmd_vel-velocity;
  pwm_num+=kp*(bias-last_bias)+ki*bias;
  last_bias=bias;
  if(pwm_num>255){pwm_num=255;}
  else if(pwm_num<0){pwm_num=0;}
}


int v(int n) //转速计算函数
{
  int vel =135*n/pusNum; // n/(pusNum*0.1)
  return vel;
}

void flash() 
{
  int wNum=ppsA;
  velocity=v(wNum); //计算速度,单位cm/s
  vel_pid(velocity,cmd_vel);
  analogWrite(pwm1,pwm_num);
  
  //打印输出相关信息
  Serial.print(pwm_num);
  if (velocity!=0)  
    {Serial.print(c);}
  if(c==aen)  //计算正位移
    {m=m+ppsA*0.034615;}
  else if(c==ben)   //计算负位移
    {m=m-ppsA*0.034615;}
  Serial.print(velocity); // 将获取的数字信号值打印到串口显示
  Serial.print("cm/s");
  Serial.print(m);
  Serial.println("cm");
  ppsA=0;    //脉冲A计数归0
}

void setup() {
  Serial.begin(9600);
  pinMode(pwm1, OUTPUT);
  attachInterrupt(0,CountA, FALLING);
  MsTimer2::set(100, flash);       // 中断设置函数，0.1s
  MsTimer2::start();   //开始计时
}
 
void loop() {
    //forward();
    //delay(100);  
 }
    
