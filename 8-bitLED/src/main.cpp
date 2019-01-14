#include <Arduino.h>
#include <FlexiTimer2.h>
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)
//#include "FlexiTimer2.h"
unsigned char AA[] = {0,0,0,1,0,0,0,0};
//共阳数码管段码
unsigned char LED_0F[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
  0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};
//共阳数码管段码DP显示
unsigned char LED_0F1[] = 
{// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
  0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf
};
unsigned char LED[8];	//用于LED的4位显示缓存
int SCLK = A3;
int RCLK = A4;
int DIO0 = A5; //这里定义了那三个脚
int DIO1 = A6; 
//int num = 0;
int num0 = 1000;//余额整数位
int num1 = 0;//余额小数位
int num2 = 0;
int num3 = 0;//扣费整数位
int num4 = 0;//扣费小数位
int temp = 0;
int year = 18;
int mon = 11;
int day = 21;
int hour = 14;
int min = 31;
int sec = 00;
int time = 0;

bool KEY_QingshuiState = 1;
bool KEY_QingshuiOld = 1;
void LED_OUT1(unsigned char X)//第一块数码管
{
  unsigned char i;
  for(i=8;i>=1;i--)
  {
    if (X&0x80) 
            {
              digitalWrite(DIO0,HIGH);
              }  
            else 
            {
              digitalWrite(DIO0,LOW);
            }
    X<<=1;
            digitalWrite(SCLK,LOW);
            digitalWrite(SCLK,HIGH);
  }
}

void LED_OUT2(unsigned char X)//第二块数码管
{
  unsigned char i;
  for(i=8;i>=1;i--)
  {
    if (X&0x80) 
            {
              digitalWrite(DIO1,HIGH);
              }  
            else 
            {
              digitalWrite(DIO1,LOW);
            }
    X<<=1;
            digitalWrite(SCLK,LOW);
            digitalWrite(SCLK,HIGH);
  }
}



void LED8_Display_deductions(void)//扣费显示
{
  unsigned char *led_table;          // 查表指针
  unsigned char i;

  if(num1 == 0)
  {
    num1 = 100;
    num0 -= 1;
  }
  
  /* //显示第1位
  led_table = LED_0F + LED[7];
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x80);				
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第2位
  led_table = LED_0F + LED[6];
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x40);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);*/

    //显示第3位
  led_table = LED_0F + (num0/1000)%10;
  i = *led_table;
  LED_OUT1(i);		
  LED_OUT1(0x20);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
 
    //显示第4位
  led_table = LED_0F + (num0/100)%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x10);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

    //显示第5位
  led_table = LED_0F + (num0/10)%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x08);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第6位
  led_table = LED_0F1 + num0%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x04);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第7位
  led_table = LED_0F + (num1/10)%10;
  i = *led_table;
  LED_OUT1(i);		
  LED_OUT1(0x02);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第8位
  led_table = LED_0F + num1%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x01);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

 
 
 /*//显示第1位
  led_table = LED_0F + 9;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x80);				
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);


  //显示第2位
  led_table = LED_0F + LED[6];
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x40);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);*/

   //显示第3位
  led_table = LED_0F +  (num3/1000)%10;
  i = *led_table;
  LED_OUT2(i);		
  LED_OUT2(0x20);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

    //显示第4位
  led_table = LED_0F + (num3/100)%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x10);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第5位
  led_table = LED_0F +(num3/10)%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x08);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第6位
  led_table = LED_0F1 + num3%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x04);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
 
   //显示第7位
  led_table = LED_0F + (num4/10)%10;
  i = *led_table;
  LED_OUT2(i);		
  LED_OUT2(0x02);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第8位
  led_table = LED_0F + num4%10;
  i = *led_table;
  LED_OUT2(i);			 
  LED_OUT2(0x01);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

}

void LED8_Display_time(void)//时间显示
{
  unsigned char *led_table;          // 查表指针
  unsigned char i;

  if(sec == 60)
    {
      sec = 0;
      min += 1;
    }
  
   //显示第1位
  led_table = LED_0F + (year/10)%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x80);				
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第2位
  led_table = LED_0F + year%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x40);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

    //显示第3位
  led_table = LED_0F + 16;
  i = *led_table;
  LED_OUT1(i);		
  LED_OUT1(0x20);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
 
    //显示第4位
  led_table = LED_0F + (mon/10)%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x10);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

    //显示第5位
  led_table = LED_0F + mon%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x08);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第6位
  led_table = LED_0F1 + 16;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x04);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第7位
  led_table = LED_0F + (day/10)%10;
  i = *led_table;
  LED_OUT1(i);		
  LED_OUT1(0x02);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第8位
  led_table = LED_0F + day%10;
  i = *led_table;
  LED_OUT1(i);			
  LED_OUT1(0x01);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

 
 
 //显示第1位
  led_table = LED_0F + (hour/10)%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x80);				
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);


  //显示第2位
  led_table = LED_0F + hour%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x40);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第3位
  led_table = LED_0F + 16;
  i = *led_table;
  LED_OUT2(i);		
  LED_OUT2(0x20);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

    //显示第4位
  led_table = LED_0F + (min/10)%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x10);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

   //显示第5位
  led_table = LED_0F + min%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x08);
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第6位
  led_table = LED_0F + 16;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x04);	
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);
 
   //显示第7位
  led_table = LED_0F + (sec/10)%10;
  i = *led_table;
  LED_OUT2(i);		
  LED_OUT2(0x02);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

  //显示第8位
  led_table = LED_0F + sec%10;
  i = *led_table;
  LED_OUT2(i);			
  LED_OUT2(0x01);		
  digitalWrite(RCLK,LOW);
  digitalWrite(RCLK,HIGH);

}
void calculate_deductions_Qingshui()//清水扣费计算
{
  if(num2 == 600)
  {
    num2 = 0;
    num1 -= 1;
    num4 += 1;
    if(num1 == 0)
    {
      num1 = 100;
      num0 -= 1;
    }
    if(num4 == 100)
    {
      num4 = 0;
      num3 += 1;
    }
   
  }
}

void calculate_time//时间计算
{
  if(time == 1000)
  {
    time = 0;
    sec += 1;
    if(sec == 60)
    {
      sec = 0;
      min += 1;
    }
    if(min == 60)
    {
      min = 0;
      hour += 1;
    }
    if(hour == 24)
    {
      hour = 0;
      day += 1;
    }
  }
}

void flash()//中断服务程序
{
  num2 += 1;
  temp += 1;
  time += 1;
  if(temp == 20)
  {
    temp = 0;
  }
  if(num2 == 1000)
  {
    num2 = 0;
  }
  if(KEY_QingshuiState == 0)
  {
    calculate_deductions_Qingshui();
  }
  calculate_time();
}

void setup ()
{
  pinMode(SCLK,OUTPUT);
  pinMode(RCLK,OUTPUT);
  pinMode(DIO0,OUTPUT); 
  pinMode(DIO1,OUTPUT);//让三个脚都是输出状态
  pinMode(53,INPUT_PULLUP);

  
  FlexiTimer2::set(1,flash);
  FlexiTimer2::start();

}
void loop()
{
 /* LED[0]=5;
  LED[1]=1;
  LED[2]=5;
  LED[3]=0;
  LED[4]=8;
  LED[5]=1;
  LED[6]=0;
  LED[7]=2;
*/
  for(int y = 0;y < 8;y++)
  {
    LED[y] = AA[7-y];
    //if(y = 8)
   // y = 0;
  }
  
  if(temp == 10)
  {
      //temp = 0;
      //delay(5);
    if(digitalRead(53) == 0)
    {
      KEY_QingshuiState = !KEY_QingshuiState;
      temp = 0;
      // delay(5);
      // KEY_QingshuiState = !KEY_QingshuiState;
    } 
  }


    //LED8_Display_deductions();
    
    LED8_Display_time();
  
  
}

