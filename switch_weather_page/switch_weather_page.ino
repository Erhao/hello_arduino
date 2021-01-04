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
const char* SSID = "xinyu";
const char* PASSWORD = "jiubugaosuni1";

// 心知天气
const char* HOST = "api.seniverse.com";
const char FINGERPRINT[] PROGMEM = "A3 8E 27 11 BE F6 D0 2A 6E 77 CC 2D 48 AF 94 F5 CC 55 76 76";
const int HTTPS_PORT = 443;
const int EXPIRE = 1800000; // 刷新时间

// 按键
const LEFT_BTN_PIN = 14;
const OK_BTN_PIN = 12;
const RIGHT_BTN_PIN = 13;
const int SHORT_PRESS_TIME = 1000;
const int LONG_PRESS_TIME  = 1000;
int last_left_btn_state = LOW;
int last_ok_btn_state = LOW;
int last_right_btn_state = LOW;
int cur_left_btn_state;
int cur_ok_btn_state;
int cur_right_btn_state;
unsigned long left_btn_pressed_time = 0
unsigned long ok_btn_pressed_time = 0
unsigned long right_btn_pressed_time = 0
unsigned long left_btn_released_time = 0
unsigned long ok_btn_released_time = 0
unsigned long right_btn_released_time = 0

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

void display_weather_page(struct Weather weather_info) {
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
  httpsClient.setFingerprint(FINGERPRINT);
  httpsClient.setTimeout(15000);
  delay(1000);

  Serial.println("HTTPS Connecting");
  int r = 0;
  while ((!httpsClient.connect(HOST, HTTPS_PORT)) && (r < 30)) {
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
  Serial.println(HOST+query_string);

  httpsClient.print(String("GET ") + query_string + " HTTP/1.1\r\n" + "Host: " + HOST + "\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("doc is:");

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

void change_weather_page(day=0) {
  struct Weather weather_info = get_weather(day);
  display_weather_page(weather_info);
}

void display_menu_page(item_index) {
  // 展示菜单页
  /*
  -----------------
  | Move & select |
  |               |
  |               |
  | * Weather     |
  | * Covid stat  |
  |               |
  -----------------
  */
  display.clearDisplay();
  display.println("    Move & select    ");
  display.println();
  display.println("     %s Weather     " % (item_index == 0 ? "*" : " "));
  display.println("     %s Covid stat  " % (item_index == 1 ? "*" : " "));
  display.display();
}

void update_weather_page_auto(expire) {
  // 自动更新天气
  uint32_t now_millis = millis();
  uint32_t millisecond = now_millis
  while (true) {
    if (millisecond - now_millis >= expire) {
      now_millis = millis();
      change_weather_page(day);
    }
    else {
      millisecond = millis();
    }
  }
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
  WiFi.begin(SSID, PASSWORD);

  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    Serial.println("Waiting...");
    display.display();
    delay(1000);
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

  update_weather_page_auto(EXPIRE);
}

void loop() {
  cur_left_btn_state = digitalRead(LEFT_BTN_PIN);
  cur_ok_btn_state = digitalRead(LEFT_OK_PIN);
  cur_right_btn_state = digitalRead(RIGHT_BTN_PIN);

  // 左键
  if (last_left_btn_state == LOW && cur_left_btn_state == HIGH) {
    left_btn_pressed_time = millis();
  } else if (last_left_btn_state == HIGH && cur_left_btn_state == LOW) {
    left_btn_released_time = millis();

    long pressDuration = left_btn_released_time - left_btn_pressed_time;
    if (pressDuration < SHORT_PRESS_TIME )
      Serial.println("[LEFT] short press is detected");
    if (pressDuration >= LONG_PRESS_TIME )
      Serial.println("[LEFT] long press is detected");
  }

  // OK键
  if (last_ok_btn_state == LOW && cur_ok_btn_state == HIGH) {
    ok_btn_pressed_time = millis();
  } else if (last_ok_btn_state == HIGH && cur_ok_btn_state == LOW) {
    ok_btn_released_time = millis();

    long pressDuration = ok_btn_released_time - ok_btn_pressed_time;
    if (pressDuration < SHORT_PRESS_TIME )
      Serial.println("[OK] short press is detected");
    if (pressDuration >= LONG_PRESS_TIME )
      Serial.println("[OK] long press is detected");
  }

  // 右键
  if (last_right_btn_state == LOW && cur_right_btn_state == HIGH) {
    right_btn_pressed_time = millis();
  } else if (last_right_btn_state == HIGH && cur_right_btn_state == LOW) {
    right_btn_released_time = millis();

    long pressDuration = right_btn_released_time - right_btn_pressed_time;
    if (pressDuration < SHORT_PRESS_TIME )
      Serial.println("[RIGHT] short press is detected");
    if (pressDuration >= LONG_PRESS_TIME )
      Serial.println("[RIGHT] long press is detected");
  }
}
