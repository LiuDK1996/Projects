#include <Arduino.h>
#include <MsTimer2.h>
int a = 0;
int b = 0;
void flash()
{
    b = digitalRead(A0);
    a += 1;
    Serial.print("第");
    Serial.print(a);
    Serial.print("次");
    Serial.print(" ");
    Serial.print("总电压");
    Serial.print(b);
    Serial.println("V");
}
void setup() {
    // put your setup code here, to run once:
    MsTimer2::set(1000, flash);
    MsTimer2::start();
    Serial.begin(9600);
    pinMode(A0,INPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
}