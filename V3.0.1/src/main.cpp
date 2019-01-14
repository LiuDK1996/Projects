#include <Servo.h> 
#include <NFC.h>
//#include <MsTimer2.h> 
#include <FlexiTimer2.h>
#include <CN_SSD1306.h>  
#include <Adafruit_ssd1306syp.h>
#include <Arduino.h>
//#include <DallasTemperature.h>
//#include <OneWire.h>

//#define ONE_WIRE_BUS A0
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);

#define SDA_PIN 12
#define SCL_PIN 11
Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN);
CN_SSD1306 lucky(SDA_PIN,SCL_PIN);

#define NOP do { __asm__ __volatile__ ("nop"); } while (0)
//输入
#define qingshui 36
#define paomo 37
#define xichen 38
#define xishou 39
#define jiesuan 40
#define zhuangtai 41
//输出
#define gaoyabeng 22
#define xichenqi 23
#define kongyaji 24
#define huanxiangfa1 25
#define huanxiangfa2 26
#define gemobeng 27
#define diancifa3 28
#define zengyabeng 29
#define diancifa2 30
#define diancifa1 31



#define fever 32 //加热


/*
 * 高压清水  gaoyabeng
 *泡沫  diancifa3 gemobeng  diancifa1 zixibeng huangxiangfa1 kongyaji
 *洗尘  xichenqi
 *洗手  diancifa2 zixibeng
 *
 *排空  huangxiangfa2 kongyaji 
 */
                                               
int a,
    b, 
    d,
    e,
    cold;
float c,
      c1,
      c2,
      c3;
int f = 0,
    g = 0,
    h = 0,
    //i = 0,
    j = 0;
int 
    time_100ms_1s = 0,
    time_100ms_2s = 0,
    time_100ms_5s = 0,
    time_100ms_60s = 0,
    time_100ms_90s = 0,
    time_100ms_300s = 0;


 
bool 
    Paikongflag = 0,
    time_100ms_1sflag = 0,
    time_100ms_2sflag = 0,
    time_100ms_5sflag = 0,
    time_100ms_60sflag = 0,
    time_100ms_90sflag = 0,
    time_100ms_300sflag = 0;

bool inittemp = 0;
int p[15];
char temp = -1;
uint8_t aaa[] = "0000000000C90400032a8a";//请求服务器当前时间？
  

 unsigned char i;


Servo myservo;//定义舵机变量名

void connectGSM(String cmd,char *res)//带应答的GSMAT命令
{
    while(1)
    {
        Serial.println(cmd);
        Serial1.println(cmd);
        delay(500);
        while(Serial1.available()>0)
        {
            if(Serial1.find(res))
            {
                delay(1000);
                return;
            }
        }
        delay(1000);
    }
}
void initGSM()
{
    connectGSM("AT","OK");//AT测试
    connectGSM("ATE1","OK");//开显示
    connectGSM("AT+CPIN?","READY");//是否插卡

}

void initGPRS()
{
    connectGSM("AT+CIPSHUT","OK");
    connectGSM("AT+CGATT=1","OK");
    connectGSM("AT+CSTT=\"CMNET\"","OK");
    connectGSM("AT+CIICR","OK");
    delay(1000);
    Serial1.println("AT+CIFSR");
    delay(1000);
    connectGSM("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"41830\"","OK");
    //connectGSM("AT+CIPSTART=\"TCP\",\"139.129.53.70s\",\"8989\"","OK");
    inittemp = 1;
}
/*
void receive()
{
    const int flag = 0;
    const int num = 0;
    while(Serial1.available() > 0)
    {
        
        temp = Serial1.read();
        delay(2);
        if(temp ==0x01 || temp == 0x00 || flag == 1)
        {
            flag = 1;
            DateBuf[num] = temp;
            num++;
            if(num == (DateBuf[6] + 7))
            {
                num = 0;
                flag = 0;
            }
        }
    }

    if(DateBuf[5] = 0xC9)//时间同步-下发时间
    {

    }

    if(DateBuf[5] = 0xCA)//读设置项-语音播报
    {
        
    }

    if(DateBuf[5] = 0xCB)//上传状态
    {
        
    }

    if(DateBuf[5] = 0x66)//账户验证-下发用户余额-语音播报
    {
        
    }

    if(DateBuf[5] = 0xCC)//预结算
    {
        
    }

    if(DateBuf[5] = 0xCD)//结算
    {
         
    }


}*/

void sendinit()
{
    Serial1.print("AT+CSTT=\"CMNET\"\r\n");
    delay(500);
    Serial1.print("AT+CIICR\r\n");
    delay(500);
    Serial1.print("AT+CIFSR\r\n");
    delay(500);
    Serial1.print("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"34176\"\r\n");
    delay(500);
    //Serial.print("");
    Serial1.print("AT+CIPSEND\r\n");
    delay(500);
    Serial1.write(aaa,22);
    Serial.println("");
    delay(500);
    Serial1.write("1A\r\n");
    delay(500);

    Serial3.print("AT+CSTT=\"CMNET\"\r\n");
    delay(5000);
    Serial3.print("AT+CIICR\r\n");
    delay(5000);
    Serial3.print("AT+CIFSR\r\n");
    delay(5000);
    Serial3.print("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"34176\"\r\n");
 
    
   
   
}


//舵机控制程序
/* 
44 水流传感
45 高水位
46 低水位

液位 有感为1 无感为0
水流 有感为0 无感为1
*/
void MG99r()
{
  if(digitalRead(46) == 0)// 低水位无水
    {
      myservo.writeMicroseconds(2495); //禁用状态 放下
      // pinMode(9, INPUT_PULLUP);
    }
    
  else 
    myservo.writeMicroseconds(1500); //正常状态 立起

  if(digitalRead(44) == 0&&digitalRead(45) == 1)//有进水 上水位有水
    myservo.writeMicroseconds(1500); //正常状态 立起
}

void LM35()
{
  
  c1 = (analogRead(A0) * 5.0 ) /1024 *100;
  delayMicroseconds(100);
  c2 = (analogRead(A0) * 5.0 ) /1024 *100;
  delayMicroseconds(100);
  c3 = (analogRead(A0) * 5.0 ) /1024 *100;
  delayMicroseconds(100);
  c = (c1+c2+c3)/3.0;
  /*if(c <= 5)
  {
    digitalWrite(fever,HIGH);
  }

  if(c >= 10)
  {
    digitalWrite(fever,LOW);
  }*/

}
//清水
void Qingshui()
{
  digitalWrite(gaoyabeng,HIGH);
  delay(200);
}
//泡沫
void Paomo()
{
  digitalWrite(diancifa3,HIGH);
  digitalWrite(gemobeng,HIGH);
  digitalWrite(diancifa1,HIGH);
  delay(200);
  digitalWrite(zengyabeng,HIGH);
  delay(200);
  digitalWrite(huanxiangfa1,HIGH);
  delay(200);
  digitalWrite(kongyaji,HIGH);
}
//泡沫停止
void PaomoStop()
{
  digitalWrite(kongyaji,LOW);
  digitalWrite(zengyabeng,LOW);
  digitalWrite(gemobeng,LOW);
  delay(100);
  digitalWrite(diancifa3,LOW);
  digitalWrite(diancifa1,LOW);
  delay(200);
  digitalWrite(huanxiangfa1,LOW);
  delay(200);

}
//洗尘
void Xichen()
{
  digitalWrite(xichenqi,HIGH);
  delay(200); 
}
//洗手
void Xishou()
{
  digitalWrite(diancifa2,HIGH);
  delay(200);
  digitalWrite(zengyabeng,HIGH);
}
//结算
void Jiesuan()
{
   for(a = 22;a < 35;a += 1)
  {
   // pinMode(a,OUTPUT);
    digitalWrite(a, LOW);
   }
}

void Paikong()
{

}

//OLED显示
void displayoled()
{
  //display.drawLine(0, 0, 127, 63,WHITE);
  //display.update();
  //delay(100);
  //display.clear();
  //display.update();
 // display.setCursor(64,0);
 // display.setTextSize(2);
 // display.setTextColor(BLACK,WHITE);
  //display.print("0x"); 
  //display.println(c);
  
  cold = d;
  //NOP; NOP; NOP; NOP; NOP; NOP;
  //NOP; NOP; NOP; NOP; NOP; NOP;
  // delay(500);
  d = c;  //温度采集
  //c = 3;
 /* if(d != cold)//温度不同刷新屏幕 已弃用 改为5S刷新
  {
    display.update();
    Serial.println(d);
  }*/

  if(d <= 5)
  {
    digitalWrite(fever,HIGH);//开加热
    for(i=5;i<9;i++)
    {
      lucky.ShowCN((i-5)*16,0,i);
    }
    //delay(500);
    display.setCursor(64,0);
    display.setTextSize(2);
    display.setTextColor(BLACK,WHITE);
    //display.print("0x"); 
    display.println(d);
    for(i=0;i<4;i++)
    {
      lucky.ShowCN(i*16,2,i);
    }
    
  }

   if(d >= 10)
  {
    digitalWrite(fever,LOW);
    for(i=5;i<9;i++)
    {
      lucky.ShowCN((i-5)*16,0,i);
    }
    //delay(500);
    display.setCursor(64,0);
    display.setTextSize(2);
    display.setTextColor(BLACK,WHITE);
    //display.print("0x"); 
    display.println(d);
    for(i=0;i<3;i++)
    {
      lucky.ShowCN(i*16,2,i);
    }
    lucky.ShowCN(3*16,2,4);
    
  }

 }


void flash()
{
  time_100ms_1s += 1;
  time_100ms_2s += 1;
  time_100ms_5s += 1;
  time_100ms_60s += 1;
   time_100ms_90s += 1;
  time_100ms_300s += 1;
  
  if(time_100ms_5s == 50)
  {
    //time_100ms_5s = 0;
    //c = (analogRead(A0) * 5.0 ) /1024 *100;  //温度采集
    //sensors.requestTemperatures(); //发送获取温度的命令
    //c = sensors.getTempCByIndex(0);
    //Serial.println(d);
    display.update();//刷新屏幕显示

  }
  if(time_100ms_1s == 15)
  {
    time_100ms_1s = 0;
  }
  if(time_100ms_2s == 25)
  {
    time_100ms_2s = 0;
  }
  if(time_100ms_5s == 55)
  {
    time_100ms_5s = 0;
  }
  if(time_100ms_60s == 605)
  {
    time_100ms_60s = 0;
  }  if(time_100ms_90s == 905)
  {
    time_100ms_90s = 0;
  }
  if(time_100ms_300s == 3005)
  {
    time_100ms_300s = 0;
  }

  
 // display.update();
 //c = (analogRead(A0) * 5.0 ) /1024 *100;  //温度采集
}

//void calculate_time()//时间计算

void setup() {
  // put your setup code here, to run once:
  for(a = 22;a < 35;a += 1)
  {
    pinMode(a,OUTPUT);
    digitalWrite(a,LOW);
   }

  // digitalWrite(35,HIGH);
   digitalWrite(34,HIGH);//宇脉供电
   //digitalWrite(27,HIGH);
    for(b = 36;b < 51;b += 1)
  {
    pinMode(b,INPUT_PULLUP);
    digitalWrite(b, HIGH);
   }

  delay(1000);
  display.initialize();
  delay(50);
  //display.update();

  myservo.attach(10 );//定义舵机接口

  Serial.begin(9600);
 // Serial1.begin(9600);//SIM800C
  Serial2.begin(115200);//NFC
  //Serial3.begin(9600);

  FlexiTimer2::set(100,flash);//100毫秒
  FlexiTimer2::start();
  NFC_int();

  //sensors.begin();


  //myservo.writeMicroseconds(2495); //禁用状态 放下

  //delay(10000);

}

void loop()
{
  // put your main code here, to run repeatedly:
 // while (digitalRead(zhuangtai) == 0)


   while (1)
  {

    NFC_loop();
    /*if(inittemp == 0)
    {
          initGSM();
          delay(1000);
          initGPRS();
    }*/
    displayoled();
    //sensors.requestTemperatures(); //发送获取温度的命令
    //c = sensors.getTempCByIndex(0);
    //display.clear();
    MG99r();
    //LM35();
    if(digitalRead(qingshui) == 0)
    {
      delay(5);
      if(digitalRead(qingshui) == 0)
      {
        Qingshui();
        Paikongflag = 0;
        if(digitalRead(qingshui) == 1)
        {
          Jiesuan();
          Paikongflag = 1;
        }
        
      }
    }
     if(digitalRead(paomo) == 0) 
     {
        delay(5);
        if(digitalRead(paomo) == 0) 
        {
          Paomo();
          Paikongflag = 0;
          if(digitalRead(paomo) == 1) 
          {
            //Jiesuan();
            PaomoStop();
            Paikongflag = 1;
           
          }
          
        } 
     }

     if(digitalRead(xishou) == 0) 
     {
       delay(5); 
       if(digitalRead(xishou) == 0)
       {
          Xishou();
          Paikongflag = 0;
          if(digitalRead(xishou) == 1) 
          {
            Jiesuan();
            Paikongflag = 1;
          }
          
        }  
       
      }
    
     if(digitalRead(xichen) == 0) 
     {
       
        delay(5);
        if(digitalRead(xichen) == 0) 
        {
          Xichen(); 
          Paikongflag = 0;
          if(digitalRead(xichen) == 1) 
          {
            Jiesuan();
            Paikongflag = 1;
          }
    
         }
      
     }
     //Paikongflag = 1;
     if((digitalRead(xichen) == 1)&&(digitalRead(xishou) == 1)&&(digitalRead(paomo) == 1)&&(digitalRead(qingshui) == 1)&&(Paikongflag == 1)&&(d <= 15))//温度小于15
     {
       time_100ms_300sflag = 1;
       //time_100ms_300s = 0;
       //Serial.println("作业结束 等待排空");
       //Serial.end();
       if((time_100ms_300sflag == 1)&&(time_100ms_300s == 3000))//停机后5分钟执行排空 开换向阀1
       {
        time_100ms_300sflag = 0;
        time_100ms_1sflag = 1;
        //time_100ms_300s == 0;
        time_100ms_1s = 0;
        digitalWrite(huanxiangfa1,HIGH);
        /*
        Serial.begin(9600);
        Serial.println("换向阀1已开");
        Serial.end();*/
       }
       if((time_100ms_1sflag ==1)&&(time_100ms_1s == 10 ))// 1S后开空压机
       {
        digitalWrite(kongyaji,HIGH);
        time_100ms_1sflag = 0;
        time_100ms_60sflag = 1;

        time_100ms_60s = 0;
        /*
        Serial.begin(9600);
        Serial.println("空压机已开");
        Serial.end();*/
      }
       if((time_100ms_60sflag == 1)&&(time_100ms_60s == 600))//开空压机后 60S关换向阀1 开换向阀2
       {
        time_100ms_60sflag = 0;
        time_100ms_90sflag = 1;
        time_100ms_90s = 0;
        digitalWrite(huanxiangfa1,LOW);
        digitalWrite(huanxiangfa2,HIGH);
        /*Serial.begin(9600);
        Serial.println("换向阀1已关 换向阀2已开");
        Serial.println(time_100ms_30s);
        Serial.end();*/
       }
       if((time_100ms_90sflag == 1)&&(time_100ms_90s == 900))//执行90S后关空压机
       {
         time_100ms_90sflag = 0;
         time_100ms_2sflag = 1;
         time_100ms_2s = 0;
         digitalWrite(kongyaji,LOW);
         /*
         Serial.begin(9600);
         Serial.println("空压机已关");
         Serial.end();*/
       }
       if((time_100ms_2sflag == 1)&&(time_100ms_2s == 20))//2S后关换向阀
       {
         time_100ms_2sflag = 0;
         digitalWrite(huanxiangfa2,LOW);
         Jiesuan();
         /*
         Serial.begin(9600);
         Serial.println("换向阀已关 排空完成");
         Serial.end();*/
         Paikongflag = 0;
       }
         
      


     }
     while(digitalRead(jiesuan) == 0)
      {  
          //delay(50);
          //if(digitalRead(jiesuan) == 1)
          Jiesuan();
        
      }

    /*
      44 高水位
      45 泡沫低液位
      46 低水位
      47 水流传感
    */

    /*
   
    if(digitalRead(44) == 1&&digitalRead(47) == 0&&digitalRead(46) == 1)//停水 低水
    {
         myservo.writeMicroseconds(1700);
    }

    if(digitalRead(44) == 1&&digitalRead(47) == 0&&digitalRead(46) == 0)//停水 无水
    {
         myservo.writeMicroseconds(1900);
    }

    if(digitalRead(44) == 1&&digitalRead(47) == 1&&digitalRead(46) == 0)//故障 
    {
         myservo.writeMicroseconds(2100);
    }

   if((digitalRead(45) == 1)&&(j == 0))//泡沫低 感应不到泡沫
    {
      //报警
        myservo.writeMicroseconds(4980);
        Serial.print ("泡沫低");
        j = 1;
    }
    if(digitalRead(45) == 0)
    {
         myservo.writeMicroseconds(1500);
          Serial.print ("泡沫正常");
          j = 0;

    }  */
   
  }
}