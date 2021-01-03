#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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

  httpClientRequest();
}

void loop() {}

// 发送HTTP请求并且将服务器响应通过串口输出
void httpClientRequest(){

  WiFiClientSecure httpsClient;

  Serial.println(host);
  Serial.println(fingerprint);

  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(15000);
  delay(1000);

  Serial.println("HTTPS Connecting");
  int r = 0;
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
    delay(100);
    Serial.print(".");
    r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }

//  String query_string = "/v7/weather/3d?location=101010100&key=829341ad458a4b9db1a54035ec4226f2";
  String query_string = "/v3/weather/daily.json?key=SwIq-1zEefmbkkHCr&location=beijing&language=en&unit=c&start=0&days=5";

  Serial.print("requesting URL: ");
  Serial.println(host+query_string);

  httpsClient.print(String("GET ") + query_string + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("resp is:");
  Serial.println("==========");

  String line;
  DynamicJsonDocument doc(1024);  
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response

    deserializeJson(doc, line);
    String loc_name = doc["results"][0]["location"]["name"];
    String date = doc["results"][0]["daily"][1]["date"];
    String text_day = doc["results"][0]["daily"][1]["text_day"];
    String high = doc["results"][0]["daily"][1]["high"];
    String low = doc["results"][0]["daily"][1]["low"];
    
    Serial.println(loc_name);
    Serial.println(text_day);
    Serial.println(high);
    Serial.println(low);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(date);
    display.setCursor(10, 20);
    display.println("wea: " + text_day);
    display.setCursor(10, 30);
    display.println("temp: " + low + " - " + high);
    display.display();
  }

//  while (httpsClient.available()) {
//    char ch = static_cast<char>(httpsClient.read());
//    Serial.print(ch);
//  }

  Serial.println("==========");
  Serial.println("closing connection");
    
  delay(5000);  //GET Data at every 5 seconds
}
