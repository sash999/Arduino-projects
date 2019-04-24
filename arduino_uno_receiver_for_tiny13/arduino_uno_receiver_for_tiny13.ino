#include <RCSwitch.h>
#include <Arduino.h>


RCSwitch mySwitch = RCSwitch();
unsigned long valueold;
unsigned long i=0;



void setup() {
    Serial.begin(9600);
    
    mySwitch.enableReceive(0); // включаем прием на 2 выводе (прерывание 0)
    Serial.println("Init complete");
}

void loop() {
  

unsigned long currentMillis = millis(); // текущий счетчик

  if (mySwitch.available()) {
    i++;
    unsigned long valuesend = mySwitch.getReceivedValue();
    Serial.print("Received data #");
    Serial.print(i);
    Serial.print(" -->");
    Serial.println((unsigned int)valuesend);
    Serial.print("temp is ");
    Serial.println(((unsigned int)valuesend - 11500) / 10.0);
 
    
    
  }
  delay(500);
  //mySwitch.resetAvailable();
  }
