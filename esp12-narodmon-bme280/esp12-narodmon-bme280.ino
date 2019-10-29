

#include "ESP8266WiFi.h"
#include <Wire.h>
#include <Adafruit_BME280.h>



//ADC_MODE(ADC_VCC); //АЦП цепляем к VCC а не к ADC0 (чтобы измерять напряжение на шине)


const char* ssid = "xxxxx";
const char* password = "xxxxx";


const char* host = "narodmon.ru";
const int httpPort = 8283;

#define DEBUG false   //писать ли дебаг в сериал
#define BLINK false  // мигать ли светодиодом

#define WIFITIMEOUT 15000           // Таймаут после которого считаем что к вифи подключиться не удалось
//#define SLEEPTIME 1800e6             // В дипслипе проводим 30 минут 
#define SLEEPTIME 600e6             // В дипслипе проводим 10 минут 
#define SENSORNAME "ESP8266_BME280"  // Имя датчика на народмоне


#define LED_BUILTIN 2            // В esp-12 встроенный светодиод на gpio2
#define LED_ON 0                 // и он инверсный
#define LED_OFF 1

Adafruit_BME280 bme;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLINK) digitalWrite(LED_BUILTIN, LED_OFF); //погасим изначально горящий для экономии энергии
  if (DEBUG) Serial.begin(115200);
  DoBlink(3,200,200);
  Wire.pins(4, 5); // gpio4-SDA, gpio5-SCL
  Wire.begin(4, 5);
  if (!bme.begin(0x76)) {  // по дефолту в либе 0x77, а у нашего китайца не так, так что задаем явно
       if (DEBUG) Serial.println("Missing BME280 sensor!");
       DoBlink(5,100,100);
       ESP.deepSleep(SLEEPTIME);
  }

  SendDataToNarodmon();
  if (DEBUG) {
   Serial.print("Going to deep sleep for ");
   Serial.print(SLEEPTIME/1e6);
   Serial.println(" seconds");
  }
  ESP.deepSleep(SLEEPTIME);
}

void DoBlink(int count, int intervalHigh, int intervalLow) {
  if(!BLINK) return;
  for (int i=count; i>0; i--) {
    digitalWrite(LED_BUILTIN, LED_ON);
    delay(intervalHigh);
    digitalWrite(LED_BUILTIN, LED_OFF);
    delay(intervalLow);
  }
}


void SendDataToNarodmon() { 
 float t,h,p,v; // Температура, влажность, давление, напряжение на батарейке
 
 //v = ESP.getVcc()/1000.0;
 //int v;
 v = (analogRead(A0) * 4.5)/1024.0;
 char msgbuffer[128]; 
 int TryToConnectTime = 0;
 
 if (DEBUG) {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 }
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED  && TryToConnectTime < WIFITIMEOUT) {
    delay(500);
    TryToConnectTime += 500;
    if (DEBUG) Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
       if (DEBUG) Serial.println("WiFi connection was not established in allowed time!");
       DoBlink(5,300, 100);
      return;
  }
  DoBlink(2,100,100);

  if (DEBUG) {
  Serial.println();
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(host);
  }
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  if (!client.connect(host, httpPort)) {
    if (DEBUG) Serial.println("connection failed");
     return;
  }
  char * mac = strdup(WiFi.macAddress().c_str());
  
  
  
  p = bme.readPressure()/133.3; 
  t = bme.readTemperature();
  h = bme.readHumidity();
  snprintf(msgbuffer,sizeof msgbuffer, "#%s#%s\n#T1#%2.2f\n#H1#%2.2f\n#U1#%2.2f\n#P1#%3.2f\n##", mac, SENSORNAME, t, h, v, p);
  //snprintf(msgbuffer,sizeof msgbuffer, "#%s#%s\n#T1#%2.2f\n#T2#%2.2f\n#H1#%2.2f\n#U1#%4u\n#P1#%3.2f\n##", mac, SENSORNAME, t_dht, t_bmp, h, v, p);
  if(DEBUG) {
    Serial.println("Sending data to narodmon.ru");
    Serial.print(msgbuffer);
    Serial.println();
  }
  
  client.print(msgbuffer);
  delay(1000);

  // читаем ответ с и отправляем его в сериал
  // вообще на ответ нужно както реагировать
  if (DEBUG)  Serial.print("Requesting: ");  
  while(client.available()){
    String line = client.readStringUntil('\r');
    if (DEBUG) {
    Serial.println("Received answer:");
    Serial.print(line);
    }
  }
  
  client.stop();
  if (DEBUG) Serial.println("Closing connection");

  WiFi.disconnect(); // отключаемся от сети
  if (DEBUG) Serial.println("Disconnect WiFi.");

    
}
void loop() {}
