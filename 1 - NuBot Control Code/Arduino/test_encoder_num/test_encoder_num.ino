
#include <Wire.h>
#include <MsTimer2.h> //中断库

int pinA=2; // 定义2号端口为A脉冲输入端口
int pinB=3;// 定义3号端口为B脉冲输入端口
int ppsA=0; //记录A脉冲的次数
int pusNum=693;
float m=0; //定义位移
char aen='+';
char ben='-';
char c; //方向
int velocity=0; //速度

  
void CountA() 
{
  if(digitalRead(pinB) == HIGH) //B脉冲为高电平
  {
    c=aen;
  }
  if(digitalRead(pinB) == LOW) //B脉冲为高电平
  {
    c=ben;
  }
  ppsA++;
} 


void setup() {
  Serial.begin(9600);
  attachInterrupt(0,CountA, FALLING);
}
 
void loop() {
    //forward();
    //delay(100);
   Serial.print(ppsA);
   Serial.println("^");
   delay(100);
 }

int v(int n) //转速计算函数
{
  int vel =67*n/pusNum; // n/(pusNum*0.1)
  return vel;
}

void flash() 
{
  
  int wNum=ppsA;
  velocity=v(wNum); //计算转速
  
  if (velocity!=0)  {
  Serial.print(c);
  }
  if(c==aen)  {
   m=m+ppsA*0.034615; //计算正位移
  }
  else if(c==ben)  {
   // m=m-ppsA*360/pusNum; //计算负位移
   m=m-ppsA*0.034615;
  }
    Serial.print(velocity); // 将获取的数字信号值打印到串口显示
    Serial.print("cm/s");
    Serial.print(m);
    Serial.println("cm");
    ppsA=0;    //脉冲A计数归0
}
    
