#include <RCSwitch.h>
#include <Arduino.h>


RCSwitch mySwitch = RCSwitch();
unsigned long valueold;




void setup() {
    Serial.begin(9600);
    
    mySwitch.enableReceive(0); // включаем прием на 2 выводе (прерывание 0)
    Serial.println("Init complete");
}

void loop() {
  
 
unsigned long currentMillis = millis(); // текущий счетчик

  if (mySwitch.available()) {
    unsigned long valuesend = mySwitch.getReceivedValue();
    Serial.print("Received data -> ");
    Serial.println((unsigned int)valuesend);
    Serial.print("temp is ");
    Serial.println(((unsigned int)valuesend - 11500) / 10.0);
 
    
    
  }
  //mySwitch.resetAvailable();
  }




