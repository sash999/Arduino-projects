/*
 * ESP8266 (NodeMCU) Handling Web form data basic example
 * <a href="https://circuits4you.com">https://circuits4you.com</a>
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <FS.h>

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
  <br><br>
  <input type="submit" value="Сохранить">
</form> 

</body>
</html>
)=====";

//SSID and Password of your WiFi router
const char* ssid = "YourSSID";
const char* password = "YourPassword";

//SSID and Password of your ESP as AP
const char* ssid_ap = "esp12-www";
const char* password_ap = "1qaz!QAZ";


ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
//===============================================================
// This routine is executed when you press submit
//===============================================================
void handleForm() {
 String firstName = server.arg("ssid"); 
 String lastName = server.arg("password"); 

 Serial.print("First Name:");
 Serial.println(firstName);

 Serial.print("Last Name:");
 Serial.println(lastName);
 
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send web page
}
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);

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
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
