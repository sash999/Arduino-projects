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

