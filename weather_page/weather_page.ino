#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

WiFiClientSecure httpsClient;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "xinyu";
const char* password = "jiubugaosuni1";
//const char* host = "devapi.qweather.com";
const char* host = "api.seniverse.com";
// 和风天气
//const char fingerprint[] PROGMEM = "F5 D5 33 D8 DA 35 EB E8 B0 FB F5 D5 B9 53 0B 82 E7 5C 0C AD";
// 心知天气
const char fingerprint[] PROGMEM = "A3 8E 27 11 BE F6 D0 2A 6E 77 CC 2D 48 AF 94 F5 CC 55 76 76";
const int httpsPort = 443;
struct Weather {
  String date;
  String text_day;
  String text_night;
  String high;
  String low;
  String rainfall;
  String wind_direction;
  String wind_speed;
  String wind_scale;
  String humidity;
};

void display_weather_page(struct Weather weather_info, int day=0) {
  // 显示天气页

  /*
  ---------------------
  | Sunny             |
  | Cloudy            |
  | SW  1  3.0km/h    |
  | -9 - 1  49%  0.0  |
  |                   |
  |    2021-01-02   0 |
  ---------------------
  */
  Serial.println(weather_info.text_day);
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(weather_info.text_day);
  display.println(weather_info.text_night);
  display.println(weather_info.wind_direction + "  " + weather_info.wind_scale + "  " + weather_info.wind_speed + "km/h");
  display.println(weather_info.low + " - " + weather_info.high + "  " + weather_info.humidity + "  " + weather_info.rainfall);
  display.setCursor(34, 56);
  display.print(weather_info.date);
  display.setCursor(116, 56);
  display.print(day);
  display.display();
}

struct Weather get_weather(int day=0) {
  //  连接Wifi
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  delay(1000);

  Serial.println("HTTPS Connecting");
  int r = 0;
  while ((!httpsClient.connect(host, httpsPort)) && (r < 30)) {
    delay(100);
    Serial.print(".");
    r++;
  }
  if (r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  String query_string = "/v3/weather/daily.json?key=SwIq-1zEefmbkkHCr&location=beijing&language=en&unit=c&start=0&days=5";

  Serial.print("requesting URL: ");
  Serial.println(host+query_string);

  httpsClient.print(String("GET ") + query_string + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("doc is:");
  Serial.println("==========");

  Weather weather_info;
  Weather day1_weather_info;
  Weather day2_weather_info;

  String line;
  DynamicJsonDocument doc(1024);
  while (httpsClient.available()) {
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
  }

  Serial.print("line: ");
  Serial.println(line);

  deserializeJson(doc, line);

  String d0_date = doc["results"][0]["daily"][day]["date"];
  String d0_text_day = doc["results"][0]["daily"][day]["text_day"];
  String d0_text_night = doc["results"][0]["daily"][day]["text_night"];
  String d0_high = doc["results"][0]["daily"][day]["high"];
  String d0_low = doc["results"][0]["daily"][day]["low"];
  String d0_rainfall = doc["results"][0]["daily"][day]["rainfall"];
  String d0_wind_direction = doc["results"][0]["daily"][day]["wind_direction"];
  String d0_wind_speed = doc["results"][0]["daily"][day]["wind_speed"];
  String d0_wind_scale = doc["results"][0]["daily"][day]["wind_scale"];
  String d0_humidity = doc["results"][0]["daily"][day]["humidity"];

  Serial.print("wd: ");
  Serial.println(d0_wind_direction);

  weather_info.date = d0_date;
  weather_info.text_day = d0_text_day;
  weather_info.text_night = d0_text_night;
  weather_info.high = d0_high;
  weather_info.low = d0_low;
  weather_info.rainfall = d0_rainfall;
  weather_info.wind_direction = d0_wind_direction;
  weather_info.wind_speed = d0_wind_speed;
  weather_info.wind_scale = d0_wind_scale;
  weather_info.humidity = d0_humidity;

  return weather_info;
}

void setup() {
  //初始化串口设置
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("init...");
  display.display();

  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);

  //开始连接wifi
  WiFi.begin(ssid, password);

  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.print("Waiting...");
    display.display();
    delay(1000);
    Serial.println("Waiting...");
  }

  display.clearDisplay();
  display.setCursor(20, 20); // left up
  display.print("Wifi OK");
  display.display();

  Serial.println("");
  Serial.print("WiFi Connected!");

  struct Weather weather_info = get_weather(0);

  Serial.println("GOT WEATHER!!!");

  display_weather_page(weather_info);
}

void loop() {
  delay(1800000);
  struct Weather weather_info = get_weather(0);
  display_weather_page(weather_info);
}
