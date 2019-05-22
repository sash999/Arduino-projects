/*
 * ESP8266 (NodeMCU) Handling Web form data basic example
 * <a href="https://circuits4you.com">https://circuits4you.com</a>
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define DEBUG true // писать ли дебаг в сериал

//const char MAIN_page[] PROGMEM = R"=====(
const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html>
<head>
   <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
   <style>
    body {background: silver;} 
   </style>
</head>
<body>

<h2>ESP12-WWW<h2>
<h3>Настройка параметров доступа к домашней сети</h3>

<form action="/action_page">
  SSID:<br>
  <input type="text" name="ssid" value="Введите SSID">
  <br>
  Password:<br>
  <input type="text" name="password" value="Введите пароль">
  <br>
  <input type="text" name="deep_sleep_time" value="1800e6">
  <br><br>
  <input type="submit" value="Сохранить">
</form> 

</body>
</html>
)=====";



//SSID and Password of your ESP as AP
const char* ssid_ap = "esp12-www";
const char* password_ap = "1qaz!QAZ";

//файлы конфигурации
const char* ssid_file = "/ssid.cfg";


ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleForm() {
 String ssid = server.arg("ssid"); 
 String password = server.arg("password"); 
 String deep_sleep_time = server.arg("deep_sleep_time"); 
 if(DEBUG) {
 Serial.print("SSID received from form is:");
 Serial.println(ssid);

 Serial.print("Password received from form is:");
 Serial.println(password);

 Serial.print("deep_sleep_time received from form is:");
 Serial.println(deep_sleep_time);
 }
 String s = "SSID is: " + ssid +"<br>Password is: " + password + "<br><br><h2>RELOAD MCU TO APPLY CHANGES</h2>";
 //String s = "<h2>RELOAD MCU TO APPLY CHANGES</h2>";
 //String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);

if(SPIFFS.begin() && DEBUG)
  {
    Serial.println("SPIFFS Initialize....ok");
  }
  else if(DEBUG)
  {
    Serial.println("SPIFFS Initialization...failed");
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
