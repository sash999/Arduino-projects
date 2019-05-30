/*
Глобальные настройки
*/
#define DEBUG true                // писать ли дебаг в сериал
#define BLINK true                // мигать ли светодиодом
#define LED_BUILTIN 2             // В esp-12 встроенный светодиод на gpio2
#define LED_ON 0                  // и он инверсный
#define LED_OFF 1                 //
#define WAIT_TIME 10000           // в мс, время ожидания соединения с домашней сетью, после истечения переключаемся в режим AP

//SSID and Password of your ESP as AP
const char* ssid_ap = "esp12-www";
const char* password_ap = "1qaz!QAZ";

//имя файла с настройками в spiffs
const char* config_file = "/config.txt";

struct mcu_config {
  String ssid; //ssid
  String pw;   //password
  String dst;  //deep sleep time 
};

