/*
 * ESP8266 (NodeMCU) Handling Web form data basic example
 * <a href="https://circuits4you.com">https://circuits4you.com</a>
 */
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RCSwitch.h>
#include "config.h"
#include "html_pages.h"



bool setup_mode; // true если мы находимся в режиме настройки параметров
LiquidCrystal_I2C lcd(0x27, 16, 2);

RCSwitch mySwitch = RCSwitch();
unsigned long i=0;

ESP8266WebServer server(80); 

void DoBlink(int count, int intervalHigh, int intervalLow) {
  if(!BLINK) return;
  for (int i=count; i>0; i--) {
    digitalWrite(LED_BUILTIN, LED_ON);
    delay(intervalHigh);
    digitalWrite(LED_BUILTIN, LED_OFF);
    delay(intervalLow);
  }
}


mcu_config read_config(const char* config_file) {
 mcu_config config; 
 File f = SPIFFS.open(config_file, "r");

 if (!f) {
    DoBlink(3,1500,500);
  }
  else
  {
      // Читаем три строки
      config.ssid = f.readStringUntil('\n');
      config.pw = f.readStringUntil('\n');
      config.dst = f.readStringUntil('\n');
      f.close();  
   }
  return config;
}

void write_config(const char* config_file, mcu_config config) {
  File f = SPIFFS.open(config_file, "w");
  
  if (!f) {
       DoBlink(3,1500,500);
          }
  else
  {
      f.print(config.ssid);
      f.write('\n');
      f.print(config.pw);
      f.write('\n');
      f.print(config.dst);
      f.write('\n');
      f.close();  
  }

  return;
}


void handleRoot() {
 mcu_config config = read_config(config_file);   
 
 String s1 = HTML_header;
 String s2 = PAGE_header;
 String s3 = "<h3>Текущие настройки:</h3> SSID : " + config.ssid + "<br>  Password : " +config.pw + "<br>DeepSleep time : " \
              + config.dst + "<br><br><h3>Новые настройки:</h3>";
 String s4 = PAGE_form;
 String s5 = HTML_footer;
 
 server.send(200, "text/html", s1+s2+s3+s4+s5); 
}

void handleConfigForm() {
 mcu_config config; 
 config.ssid = server.arg("ssid"); 
 config.pw = server.arg("password"); 
 config.dst = server.arg("deep_sleep_time");
 
 String s1 = HTML_header;
 String s2 = "Сохранено в SPIFFS:<br><br>SSID : " + config.ssid + "<br>Password : " + config.pw + "<br>DeepSleep time : " \
              + config.dst + "<br><br><h2>ПЕРЕЗАГРУЗИТЕ МИКРОКОНТРОЛЛЕР ЧТОБЫ ПРИМЕНИТЬ</h2>";
 String s3 = HTML_footer;

 write_config(config_file, config);
 server.send(200, "text/html", s1+s2+s3); 
 
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  
   Serial.begin(9600);
   Wire.begin(D6, D5); // LCD1602 SCL on D5, SDA on D6
   pinMode(LED_BUILTIN, OUTPUT);
   mySwitch.enableReceive(0); // включаем прием на 2 выводе (прерывание 0) !! у lolin nodemcu это D3 !!
   
   lcd.init();
   lcd.backlight();
   lcd.home();
   lcd.print("NodeMCU");
   lcd.setCursor(0,1);
   lcd.print("   base station   ");
   DoBlink(5,200,200);
   
   
if(SPIFFS.begin() && DEBUG)
  {
    Serial.println("SPIFFS Initialize....ok");
  }
  else if(DEBUG)
  {
    Serial.println("SPIFFS Initialization...failed");
    DoBlink(10,300,300);
  }

  mcu_config config = read_config(config_file);  


  WiFi.begin(config.ssid, config.pw);  
  if(DEBUG) {   
    Serial.print("Attempting to connect to ");
    Serial.println(config.ssid);
    Serial.print("with password ");
    Serial.println(config.pw);
   }
  // Wait for connection
  int waittime = 0;
  lcd.clear();
  lcd.home();
  lcd.print("Connecting");
  lcd.setCursor(0,1);
  while (WiFi.status() != WL_CONNECTED && waittime < WAIT_TIME) {
    delay(1000);
    waittime += 1000;
    lcd.print(".");
    if(DEBUG) Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
  //If connection successful show IP address in serial monitor
  if(DEBUG) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println("WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  
  }
  lcd.clear();
  lcd.home();
  lcd.print("NodeMCU ready");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  setup_mode = false;   
  } else {
  //bring up our own access point 
  if(DEBUG) Serial.println("\nConnect failed.\nBringing up our own AP.");
  WiFi.disconnect(true);
  WiFi.softAP(ssid_ap, password_ap);
  delay(500);
  if(DEBUG) {
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
   }  
  lcd.clear();
  lcd.home();
  lcd.print("   SETUP MODE   ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.softAPIP());

   setup_mode = true;
   server.on("/", handleRoot);
   server.on("/action_page", handleConfigForm); 
   }
  
  if(setup_mode) {
   server.begin(); 
   if(DEBUG) Serial.println("HTTP server started");
  }
    
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  if(setup_mode) server.handleClient();          //
// прием данных от удаленных датчиков 433mhz
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
   
  mySwitch.resetAvailable();  
  }
  
  delay(1000);
  Serial.println("loop 1 sec");
  if(setup_mode && DEBUG) Serial.println("---SETUP MODE---");
}
