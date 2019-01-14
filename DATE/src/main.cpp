#include <Arduino.h>
#include <NFC.h>
bool inittemp = 0;
int p[15];
char temp = -1;
uint8_t aaaa[] = "0000000000C90400032a8a";//请求服务器当前时间？
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
    Serial1.print("AT+RESET");
    delay(5000);
    connectGSM("AT","OK");//AT测试
    connectGSM("ATE0","OK");//关显示
    //connectGSM("AT+CPIN?","READY");//是否插卡

}

void initGPRS()
{
    connectGSM("AT+CIPSHUT","OK");
    connectGSM("AT+CGATT=1","OK");
    //connectGSM("AT+CIPMODE=1","OK");
    connectGSM("AT+CSTT=\"CMNET\"","OK");
    connectGSM("AT+CIICR","OK");
    //delay(1000);
    Serial1.println("AT+CIFSR");
    Serial1.println("AT+CIPQSEND=0");//0 慢发模式 1快发模式
    Serial1.println("AT+CIPSPRT=2");//2不显示>和SEND
    //delay(1000);
    //connectGSM("AT+CIPSTART=\"TCP\",\"206i9l0870.imwork.net\",\"51069\"","OK");
    connectGSM("AT+CIPSTART=\"TCP\",\"139.129.53.70\",\"8989\"","OK");
    //delay(1000);
    //connectGSM("AT+CIPSTART=\"TCP\",\"tt.ai-thinker.com\",\"43498\"","OK");
    //connectGSM("AT+CIPSTART=\"TCP\",\"139.129.53.70s\",\"8989\"","OK");
    //inittemp = 1;
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

void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial1.begin(115200);//SIM800C
    Serial2.begin(115200);//NFC
    //Serial3.begin(9600);
    //delay(5000);
    //sendinit();
    //delay(500);
    
}


void loop()
{
   
    //digitalWrite(13,HIGH);
    //delay(500);
   // digitalWrite(13,LOW);
  
    
   if(inittemp == 0)
   {
        initGSM();
        delay(1000);
        initGPRS();
   }
    
    NFC_loop();
   
           
            
        
     /*while(Serial1.available()>0)
      {
        unsigned char i;
        if(Serial.find("0x00"))
        {
            for(i=0;i<15;i++) 
            {
                receive_GPRS[i]= Serial1.read();
                Serial.write(receive_GPRS[i]);
            }
        } 
      }*/
      
     /* while(Serial1.available()>0)
      {
        unsigned char i = Serial1.read();
        if(i == "0x00")
        {
          for(i=0;i<15;i++) 
          {
            receive_GPRS[i]= Serial1.read();
            Serial.write(receive_GPRS[i]);
          }
        }
        
        
      }*/
}