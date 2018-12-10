#include <Arduino.h>
#include <NFC.h>
bool inittemp = 0;
int p[15];
char temp = -1;
uint8_t aaa[] = "0000000000C90400032a8a";//请求服务器当前时间？
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
    Serial.print("AT+CSTT=\"CMNET\"\r\n");
    delay(500);
    Serial.print("AT+CIICR\r\n");
    delay(500);
    Serial.print("AT+CIFSR\r\n");
    delay(500);
    Serial.print("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"34176\"\r\n");
    delay(500);
    //Serial.print("");
    Serial.print("AT+CIPSEND\r\n");
    delay(500);
    Serial.write(aaa,22);
    Serial.println("");
    delay(500);
    Serial.write("1A\r\n");
    delay(500);

    Serial3.print("AT+CSTT=\"CMNET\"\r\n");
    delay(5000);
    Serial3.print("AT+CIICR\r\n");
    delay(5000);
    Serial3.print("AT+CIFSR\r\n");
    delay(5000);
    Serial3.print("AT+CIPSTART=\"TCP\",\"122.114.122.174\",\"34176\"\r\n");
 
    
   
   
}
void setup() 
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.begin(9600);//SIM800C
    Serial2.begin(115200);//NFC
    Serial3.begin(9600);
    //delay(5000);
    //sendinit();
    //delay(500);
    
}


void loop()
{
   /*
    digitalWrite(13,HIGH);
    delay(500);
    digitalWrite(13,LOW);
    NFC_loop();
    */
   if(inittemp == 0)
   {
        initGSM();
        delay(1000);
        initGPRS();
   }


   /* while(Serial1.available()>0)
        {
          
            //p = Serial1.read();
            //p = 11;
            for(int atemp = 0;atemp < 5;atemp++)
            {
                p[atemp] = Serial1.read();
                Serial.println(p[atemp],HEX);
                
            }
           
            
        }
        */
}