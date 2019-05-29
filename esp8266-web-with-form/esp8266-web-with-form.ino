/*
 * ESP8266 (NodeMCU) Handling Web form data basic example
 * <a href="https://circuits4you.com">https://circuits4you.com</a>
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>
//#include <string.h>

#define DEBUG true                // писать ли дебаг в сериал
#define BLINK true                // мигать ли светодиодом
#define LED_BUILTIN 2             // В esp-12 встроенный светодиод на gpio2
#define LED_ON 0                  // и он инверсный
#define LED_OFF 1                 //


const char* HTML_header = R"=====(
<!DOCTYPE html>
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
   <style>
    body {background: silver;} 
   </style>
</head>
<body>
)=====";

const char * HTML_footer = R"=====(
</body>
</html>
)=====";

const char PAGE_header[] = R"=====(
<h2>ESP12-WWW</h2>
Настройка параметров доступа к домашней сети<br>

)=====";
const char PAGE_form[] = R"=====(

<form action="/action_page">
  SSID:<br>
  <input type="text" name="ssid" value="">
  <br>
  Password:<br>
  <input type="text" name="password" value="">
  <br>
  Deep sleep time:<br>
  <input type="text" name="deep_sleep_time" value="">
  <br><br>
  <input type="submit" value="Сохранить">
</form> 
)=====";



//SSID and Password of your ESP as AP
const char* ssid_ap = "esp12-www";
const char* password_ap = "1qaz!QAZ";

//файл с настройками
const char* config_file = "/config.txt";

struct mcu_config {
  String ssid;
  String pw;  //password
  String dst; //deep sleep time 
};

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
    Serial.println("file open failed");
    DoBlink(3,500,500);
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
    Serial.println("file open failed");
    DoBlink(3,500,500);
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

  
  
 if(DEBUG) Serial.print("Client connected.");
/*
File f = SPIFFS.open(config_file, "r");
  
  if (!f) {
    Serial.println("file open failed");
  }
  else
  {
      Serial.println("Reading Data from File:");
      // Читаем три строки
      config.ssid = f.readStringUntil('\n');
      config.pw = f.readStringUntil('\n');
      config.dst = f.readStringUntil('\n');
      Serial.println(config.ssid);
      Serial.println(config.pw); 
      Serial.println(config.dst); 
      f.close();  //Close file
      Serial.println("File Closed");
  }
 */
 String s1 = HTML_header;
 
 String s2 = PAGE_header;

 String s3 = "<h3>Текущие настройки:</h3> SSID : " + config.ssid + "<br>  Password : " +config.pw + "<br>DeepSleep time : " \
              + config.dst + "<br><br><h3>Новые настройки:</h3>";
 
 String s4 = PAGE_form;
 String s5 = HTML_footer;
 
 server.send(200, "text/html", s1+s2+s3+s4+s5); 
 
}

void handleForm() {
 mcu_config config; 
 config.ssid = server.arg("ssid"); 
 config.pw = server.arg("password"); 
 config.dst = server.arg("deep_sleep_time");
 
 if(DEBUG) {
 Serial.print("SSID received from form is:");
 Serial.println(config.ssid);

 Serial.print("Password received from form is:");
 Serial.println(config.pw);

 Serial.print("deep_sleep_time received from form is:");
 Serial.println(config.dst);

 }
 String s1 = HTML_header;
 String s2 = "Сохранено в SPIFFS:<br><br>SSID : " + config.ssid + "<br>Password : " + config.pw + "<br>DeepSleep time : " \
              + config.dst + "<br><br><h2>ПЕРЕЗАГРУЗИТЕ МИКРОКОНТРОЛЛЕР ЧТОБЫ ПРИМЕНИТЬ</h2>";
 String s3 = HTML_footer;

 write_config(config_file, config);
 server.send(200, "text/html", s1+s2+s3); //Send web page
 /*
 File f = SPIFFS.open(config_file, "w");
  
  if (!f) {
    Serial.println("file open failed");
    DoBlink(3,500,500);
  }
  else
  {
      //Write data to file
      Serial.println("Writing Data to File");
      f.print(ssid);
      f.write('\n');
      f.print(password);
      f.write('\n');
      f.print(deep_sleep_time);
      f.write('\n');
      f.close();  //Close file
  }
*/
 
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
   pinMode(LED_BUILTIN, OUTPUT);
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


/*
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
*/
  WiFi.softAP(ssid_ap, password_ap);
  delay(500);
  server.on("/", handleRoot);     
  server.on("/action_page", handleForm); 

  server.begin(); 
  if(DEBUG) {                
   Serial.println("HTTP server started");
    }
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
