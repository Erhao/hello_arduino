#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.print("1line");
  display.println("xinyu");

  display.setTextSize(1);
  display.setCursor(20, 20); // left up
  display.print("Hello, world!");


  display.setTextSize(0.8); // 1 actually
  display.print("size 0.8");

  display.setTextSize(0.5); // 1 actually
  display.print("size 0.5");

  display.setTextSize(0.3); // 1 actually
  display.println("size 0.3");

  display.println("dis1");
  display.display();

  display.println("dis2");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:

}
