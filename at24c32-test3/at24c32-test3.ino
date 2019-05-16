#include <Wire.h>
#include <string.h>
#define AT24C32 0x50 // i2c address of eeprom

const int maxlen = 30; // максимальная длина строки для записи - 30 байт (29 символов плюс конец строки)
char* ssid = "THIS_IS_SSID_LOOOOOOOONG_SSID";
char* password = "THIS_IS_PASSw0RD";
char* ssid2 = "THIS_IS_SSID_LOOOOOOONG_SSID2";

void write_to_eeprom(byte highAddressByte, byte lowAddressByte, char* data)
{
  Wire.beginTransmission(AT24C32);
  Wire.write(highAddressByte);      //First Word Address
  Wire.write(lowAddressByte);      //Second Word Address
  //Serial.print("data len to write is ");
  //Serial.println(strlen(data));
  
  for(byte i=0; i<strlen(data); i++)    
  {
    Wire.write(data[i]);    
   }
  /* можно и без stdio.h
  byte i=0;
  while(data[i] != 0x00) {
     Wire.write(data[i]);
     i++;
  }
  */
 
 delay(10);
 Wire.endTransmission();
 delay(10);
}

char* read_from_eeprom(byte highAddressByte, byte lowAddressByte)
{
  Wire.beginTransmission(AT24C32);
  Wire.write(highAddressByte);      //First Word Address
  Wire.write(lowAddressByte);      //Second Word Address
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(AT24C32, maxlen);
  delay(10);
  char* readed_data = (char*)malloc(maxlen);
 
 for(byte i=0; i<maxlen; i++)      //Read 30 bytes
 {
   readed_data[i] = Wire.read();
   if(readed_data[i] == 0x00) break;
 }
 delay(10);
 return(readed_data);
}



byte highAddressByte(word address)
{
  byte BYTE_1;
  BYTE_1 = address >> 8;
  return BYTE_1;
}

byte lowAddressByte(word address)
{
  byte BYTE_1;
  byte BYTE_2;
  BYTE_1 = address >> 8;
  BYTE_2 = address - (BYTE_1 << 8);
  //BYTE_2 = address & 0xFF;
  return BYTE_2;
}


void setup()
{ 
 Wire.begin();
 Serial.begin(9600);
 delay(1000); 
 
 //WRITE!!!!*******************************
 Serial.println("Writing to EEPROM following data: ");
 Serial.println(ssid);
 Serial.println(password);
 Serial.println(ssid2);
 
 write_to_eeprom(highAddressByte(0), lowAddressByte(0), ssid);
 write_to_eeprom(highAddressByte(32), lowAddressByte(32), password);
 write_to_eeprom(highAddressByte(64), lowAddressByte(64), ssid2);
 
 //READ!!!!*********************************
 Serial.println("Reading from EEPROM:");

 char* readed_ssid = read_from_eeprom(highAddressByte(64), lowAddressByte(64));
 char* readed_pw = read_from_eeprom(highAddressByte(32), lowAddressByte(32));
 char* readed_ssid2 = read_from_eeprom(highAddressByte(64), lowAddressByte(64));
 
 Serial.println(readed_ssid);
 Serial.println(readed_pw);
 Serial.println(readed_ssid2);
}

void loop()
{
}
