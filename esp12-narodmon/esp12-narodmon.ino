

#include "ESP8266WiFi.h"
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>



//ADC_MODE(ADC_VCC); //АЦП цепляем к VCC а не к ADC0 (чтобы измерять напряжение на шине)


const char* ssid = "TP_SMALL";
const char* password = "2wsx@WSX";
/*
const char* ssid = "KeeUltra";
const char* password = "xxxxxxxxxx";
*/
const char* host = "narodmon.ru";
const int httpPort = 8283;

#define DEBUG false   //писать ли дебаг в сериал
#define BLINK false  // мигать ли светодиодом

#define WIFITIMEOUT 15000           // Таймаут после которого считаем что к вифи подключиться не удалось
#define SLEEPTIME 1800e6             // В дипслипе проводим 30 минут 
#define SENSORNAME "ESP8266_BMP180_DHT22"  // Имя датчика на народмоне


#define DHTPIN 12                // Пин к которому подключен датчик (gpio12 Он же D6)
#define DHTTYPE DHT22            // Используемый датчик DHT 22

#define LED_BUILTIN 2            // В esp-12 встроенный светодиод на gpio2
#define LED_ON 0                 // и он инверсный
#define LED_OFF 1

DHT dht(DHTPIN, DHTTYPE);       
Adafruit_BMP085 bmp;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  if (!BLINK) digitalWrite(LED_BUILTIN, LED_OFF); //погасим изначально горящий для экономии энергии
  if (DEBUG) Serial.begin(115200);
  DoBlink(3,200,200);
  Wire.pins(4, 5); // gpio4-SDA, gpio5-SCL
  Wire.begin(4, 5);
  if (!bmp.begin()) {
       if (DEBUG) Serial.println("Missing BMP180 sensor!");
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
 float t_dht,t_bmp,h,p; // Температура DHT11, температура BMP180, влажность, давление
 float v; //напрядение питания 
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
       DoBlink(1,100, 100);
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
  //t_dht = dht.readTemperature(); //TODO - может быть nan, надо проверять!
  //h = dht.readHumidity();    //TODO - может быть nan, надо проверять!
  
  // делаем максимум 5 попыток считать данные с DHT22
  for(int j=0; j<5; j++ ) {
    t_dht = dht.readTemperature();
    if(!isnan(t_dht)) break;
  }
  
  for(int j=0; j<5; j++ ) {
    h = dht.readHumidity();
    if(!isnan(h)) break;
  }
  
  
  p = bmp.readPressure()/133.3;
  t_bmp = bmp.readTemperature();
  snprintf(msgbuffer,sizeof msgbuffer, "#%s#%s\n#T1#%2.2f\n#T2#%2.2f\n#H1#%2.2f\n#U1#%2.2f\n#P1#%3.2f\n##", mac, SENSORNAME, t_dht, t_bmp, h, v, p);
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
