#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 测试HTTP请求用的URL。注意网址前面必须添加"http://"
//#define URL "https://httpbin.org/get"
#define URL "http://api.tianapi.com/txapi/ncovcity/index?key=d75d29d93081d5e5f85496824d7707b6"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "xinyu";
const char* password = "jiubugaosuni1";

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

  display.setCursor(0, 0);
  display.println("Wifi OK");
  display.println("Thundershower with Hail");
  display.println("abcdefghijklmnopqrstuvwxyz");
  display.println("1234567890-1234567890-1234567890-1234567890-");
  display.println("line5");
  display.println("line6");
  display.println("line7");
  display.println("line8");
  display.display();

  Serial.println("");
  Serial.print("WiFi Connected!");
}

void loop() {}
