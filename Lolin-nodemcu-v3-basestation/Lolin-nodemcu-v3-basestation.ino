#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RCSwitch.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RCSwitch mySwitch = RCSwitch();
unsigned long i=0;

void setup() {

Serial.begin(9600);
mySwitch.enableReceive(0); // включаем прием на 2 выводе (прерывание 0)


Wire.begin(D6, D5); // LCD1602 SCL on D1, SDA on D2

lcd.init();
lcd.backlight();

lcd.home();

lcd.print("NodeMCU");
lcd.setCursor(0,1);
lcd.print("   base station   ");
Serial.println("Init complete");

}

void loop() { 
  if (mySwitch.available()) {
    lcd.clear();
    i++;
    unsigned long valuesend = mySwitch.getReceivedValue();
    Serial.print("Received data #");
    Serial.print(i);
    Serial.print(" -->");
    Serial.println((unsigned int)valuesend);
    Serial.print("temp is ");
    Serial.println(((unsigned int)valuesend - 11500) / 10.0);
    lcd.setCursor(0,0);
    lcd.print(i);
    lcd.setCursor(0,1);
    lcd.print(((unsigned int)valuesend - 11500) / 10.0);
 
    
    
  }
  delay(5000);
  
  
  
  
  }
