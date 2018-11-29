#include <Servo.h> 
#include <Adafruit_ssd1306syp.h>
#define SDA_PIN 8
#define SCL_PIN 9
Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN);

//输入
#define qingshui 36
#define paomo 37
#define xichen 38
#define xishou 39
#define jiesuan 40
#define zhuangtai 41
//输出
#define gaoyabeng 22
#define kongyaji 24
#define zengyabeng 29
#define gemobeng 27
#define diancifa1 31
#define huanxiangfa1 25
#define diancifa2 30
#define xichenqi 23
#define diancifa3 28


/*
 * 高压清水  gaoyabeng
 *泡沫  diancifa3 gemobeng  diancifa1 zixibeng huangxiangfa1 kongyaji
 *洗尘  xichenqi
 *洗手  diancifa2 zixibeng
 *
 */
                                               
int a,
    b, 
    c,
    d,
    e;

int f = 0,
    g = 0,
    h = 0,
    i = 0,
    j = 0;

Servo myservo;
//舵机控制程序
void MG99r()
{
   if(digitalRead(44) == 0)
    {
      myservo.writeMicroseconds(2495);
      // pinMode(9, INPUT_PULLUP);
    }
    
  else 
   myservo.writeMicroseconds(1500);
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

//OLED显示
void displayoled()
{
   display.drawLine(0, 0, 127, 63,WHITE);
  display.update();
  delay(1000);
  display.clear();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("ERROR");
  display.setTextColor(BLACK, WHITE);
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //display.print("0x"); 
  display.println(c, HEX);
  display.update();
  delay(2000);
  display.clear();
 }
void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   myservo.attach(10);//舵机接口
   pinMode(13,OUTPUT);
   
  for(a = 22;a < 35;a += 1)
  {
    pinMode(a,OUTPUT);
    digitalWrite(a,LOW);
   }
   
   digitalWrite(34,HIGH);//宇脉供电

    for(b = 36;b < 51;b += 1)
  {
    pinMode(b,INPUT_PULLUP);
    digitalWrite(b, HIGH);
   }

  delay(1000);
  display.initialize();
  
}

void loop()
{
  // put your main code here, to run repeatedly:
 // while (digitalRead(zhuangtai) == 0)
   while (1)
  {
  
    //delay(1000);
    c = (analogRead(A0) * 5.0 ) /1024 *100;  //温度采集
    //displayoled();
    //MG99r();

    if(digitalRead(qingshui) == 0)
    {
        delay(5);
        if(digitalRead(qingshui) == 0)
        {
            Qingshui();
            if(digitalRead(qingshui) == 1)
            Jiesuan();
        }
    }
    if(digitalRead(paomo) == 0) 
    {
        delay(5);
        if(digitalRead(paomo) == 0) 
        {
            Paomo();
            if(digitalRead(paomo) == 1) 
            Jiesuan();
        } 
    }

    if(digitalRead(xishou) == 0) 
    {
        delay(5); 
        if(digitalRead(xishou) == 0)
        {
            Xishou();
            if(digitalRead(xishou) == 1) 
            Jiesuan();
        }  
    }

    if(digitalRead(xichen) == 0) 
    {
        delay(5);
        if(digitalRead(xichen) == 0) 
        {
            Xichen(); 
            if(digitalRead(xichen) == 1) 
            Jiesuan(); 
        }
    }
    if(digitalRead(jiesuan) == 0)
    {  
        //delay(50);
        //if(digitalRead(jiesuan) == 1)
        Jiesuan();
    }

    if(digitalRead(44) == 0)//进水
    {
         myservo.writeMicroseconds(1500);
    }


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

    }

  } 
  
}

