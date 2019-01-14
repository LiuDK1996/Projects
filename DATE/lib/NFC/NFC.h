unsigned char icID[] = {0xBB,0xe1,0xf1,0x9b};//IC卡号
unsigned char machineID[] = {0x00,0x03,0x0d,0x4c};//机器号 200012

unsigned char send1[11] = {0x00,0x00,0x00,0x00,0x00,0xC9,0x04,0x00,0x00,0x00,0x00};//时间同步
unsigned char send2[11] = {0x00,0x00,0x00,0x00,0x00,0xCA,0x04,0x00,0x00,0x00,0x00};//读设置项
unsigned char send3[27] = {0x00,0x00,0x00,0x00,0x00,0xCB,0x1C,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//上传状态
unsigned char send4[15] = {0x00,0x00,0x00,0x00,0x00,0x66,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//账户验证
unsigned char send5[19] = {0x00,0x00,0x00,0x00,0x00,0xCC,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//预结算
unsigned char send6[19] = {0x00,0x00,0x00,0x00,0x00,0xCD,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//结算
const unsigned char wake[24]={
  0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0xfd, 0xd4, 0x14, 0x01, 0x17, 0x00};//NFC唤醒
const unsigned char tag[11]={
  0x00, 0x00, 0xFF, 0x04, 0xFC, 0xD4, 0x4A, 0x01, 0x00, 0xE1, 0x00};//获取UID
const unsigned char std_ACK[25] = {
  0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x0C, \
0xF4, 0xD5, 0x4B, 0x01, 0x01, 0x00, 0x04, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x4b, 0x00};//获取UID后返回
unsigned char old_id[5];
unsigned char new_id[5];
unsigned char receive_ACK[25];//接收缓存
unsigned char receive_GPRS[100];//接收缓存
int UID[25];
String aaa = "";
#include "Arduino.h"
#define print1Byte(args) Serial2.write(args)
#define print1lnByte(args)  Serial2.write(args),Serial2.println()

void send_1()
{
  Serial1.print("AT+CIPSEND=11\r\n");//11位 
  delay(50);
  int z;
  /*for(y = 0;y < 4;y++)
  {
    send1[y+7] = new_id[y];
  }*/
  for (z = 0;z < 11;z++)
  {
    //Serial.print(send1[z],HEX);
    //Serial1.write(send1[z]);
    Serial1.write(send1[z]);
    //Serial.print(" ");
  }
  
}

void send_2()
{
  Serial1.print("+++\r\n");
  Serial1.print("AT+CIPSEND = 11\r\n");
  delay(500);
  int y,z;
  for(y = 0;y < 4;y++)
  {
    send2[y+7] = machineID[y];
  }
  for (z = 0;z < 11;z++)
  {
    Serial1.write(send2[z]);
    //Serial.write(send2[z]);
    //Serial.print(" ");
  }
   Serial1.write("1A\r\n");
  //Serial.println();
}



void send_4()//发送send4数据
{
  Serial1.print("+++\r\n");
  Serial1.print("AT+CIPSEND = 15\r\n");
  delay(500);
  int z;
  for (z = 0;z < 15;z++)
  {
    Serial1.write(send4[z]);
   // Serial.write(send4[z]);
    //Serial.print(" ");
  }
  //Serial1.write("1A\r\n");
 // Serial.println();

}
void UART_Send_Byte(unsigned char command_data)
{//send byte to PC
  Serial.print(command_data,HEX);
  Serial.print(" ");
} 
void UART1_Send_Byte(unsigned char command_data)
{//send byte to device
  print1Byte(command_data);
  Serial2.flush();// complete the transmission of outgoing serial data 
} 
void read_ACK(unsigned char temp)//读取NFC缓存
{//read ACK into reveive_ACK[]
  unsigned char i;
  for(i=0;i<temp;i++) 
  {
    receive_ACK[i]= Serial2.read();
  }
}

void read_GRRS(unsigned char temp)//读取GPRS缓存 前两位为换行符 应当舍弃
{//read ACK into reveive_GPRS[]
  unsigned char i;
  for(i=0;i<temp;i++) 
  {
    delay(2);
    receive_GPRS[i]= Serial1.read();
  }
  
  for(i=2;i<temp;i++) 
  {
    Serial.write(receive_GPRS[i]);
  }
}
void wake_card(void)//唤醒
{//send wake[] to device
  unsigned char i;
  for(i=0;i<24;i++) //send command
    UART1_Send_Byte(wake[i]);
}
void send_tag(void)//UID
{//send tag[] to device
  unsigned char i;
  for(i=0;i<11;i++) //send command
    UART1_Send_Byte(tag[i]);
}
void copy_id (void) //复制UID
{//save old id
  int ai, oi;
  for (oi=0, ai=19; oi<5; oi++,ai++) {
    old_id[oi] = receive_ACK[ai];
  }
 /* int x;
  for(x = 0;x < 5;x++)
  {
    new_id[x] = old_id[4-x];
  }
  */
}

char cmp_id (void) //测重
{//return true if find id is old
  int ai, oi;
  for (oi=0,ai=19; oi<5; oi++,ai++) {
    if (old_id[oi] != receive_ACK[ai])
      return 0;
  }
  return 1;
}

int test_ACK (void) //信息比对
{// return true if receive_ACK accord with std_ACK
  int i;
  for (i=0; i<19; i++) {
    if (receive_ACK[i] != std_ACK[i])
      return 0;
  }
  return 1;
}

void copy_UID()//复制ID号到send1
{
 int x;
  for (x = 7;x <= 11;x++)
  {
    send1[x] = machineID[x-7];
  }
  
  //send1[7] = receive_ACK[22];
  //send1[8] = receive_ACK[21];
  //send1[9] = receive_ACK[20];
  //send1[10] = receive_ACK[19];

}

void send_id (void) //打印UID
{//send id to PC
  int i;
  Serial.print ("ID: ");
  for (i=19; i<= 22; i++) {
    Serial.print (receive_ACK[i], HEX);
    Serial.print (" ");
  }
  Serial.println ();
}

void NFC_int()
{
  Serial2.begin(115200);//NFC
  wake_card();
  delay(100);
  read_ACK(15);
  delay(100);
}

/*void NFC_loop()
{   
    send_tag(); 
    read_ACK(25);
    delay(100);
    if (!cmp_id()) 
    {
        if (test_ACK()) 
        {
            //display (25);
            // send_ID();
            //send_id();
            copy_UID();
            //send_4();//账户验证
            send_id();
            delay(50);
        }
    }
  copy_id();

}*/
void NFC_loop() 
{
  send_tag(); 
  read_ACK(25);
  delay(50);
  if(!cmp_id()) 
  {
    if(test_ACK()) 
    {
      
      copy_UID();
      while(Serial1.available()>0)
      {
        unsigned char i = 0;
        receive_GPRS[i]= Serial1.read();
        //Serial.write(receive_GPRS[i]);
        i++;
        //Serial.println(Sderial1.available());   
      }
      send_1();
      delay(1000);
      read_GRRS(15);
      
    /*  while(Serial1.available()>0)
      {
        unsigned char i = 0;
        receive_GPRS[i]= Serial1.read();
        Serial.write(receive_GPRS[i]);
        i++;
        //Serial.println(Sderial1.available());   
      }
       send_1();
      while(Serial1.available()>0)
      {
        unsigned char i = 0;
        receive_GPRS[i]= Serial1.read();
        Serial.write(receive_GPRS[i]);
        i++;
        //Serial.println(Sderial1.available());   
      }*/
      

      //send_id();
      //delay(100);
    }
  }
  copy_id();
  
  
}