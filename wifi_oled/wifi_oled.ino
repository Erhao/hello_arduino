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

  //重点1 创建 HTTPClient 对象
  HTTPClient httpClient;


  //重点2 通过begin函数配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL);
  Serial.print("URL: "); Serial.println(URL);

  //重点3 通过GET函数启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);

  //重点4. 如果服务器响应HTTP_CODE_OK(200)则从服务器获取响应体信息并通过串口输出
  //如果服务器不响应HTTP_CODE_OK(200)则将服务器响应状态码通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    // 使用getString函数获取服务器响应体内容
    String responsePayload = httpClient.getString();
    Serial.println("Server Response Payload: ");
    Serial.println(responsePayload);
  } else {
    Serial.println("Server Response Code：");
    Serial.println(httpCode);
  }

  //重点5. 关闭ESP8266与服务器连接
  httpClient.end();
}
