/******************************************************************
* 程序名称：按键测试程序
* 程序功能：按下按键程序执行不同脚本，并通过串口显示程序执行模式
* 示例程序：示例程序为 4 种模式，可任意进行修改，支持 0~9 种模式
* 程序版本：Ver 1.0
* 编写日期：2020/07/30
* 作者信息：天津科技大学创新实验室-JC
******************************************************************/

#define   BTN 14   // 按键连接引脚

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  int KeyNum = 0;
  int BTN_val = digitalRead(BTN);

  if (digitalRead(BTN) == 1) {               // 如果检测到按键按下
    uint32_t now_millis = millis();
    Serial.println(now_millis);
    Serial.println("detected key down");
    while (true) {
      if (millis() - now_millis > 1000) {
        break;
      }
      if (digitalRead(BTN) == 1) {
        Serial.println("key down");
        KeyNum ++;
        delay(100);
      }
    }
    Serial.print("KeyNum:");
    Serial.println(KeyNum);
    if (KeyNum>=5) {                          // 如果超过设定执行状态数量，那么就回到 1 号模式
      KeyNum = 1;
      }
    }

//  switch(KeyNum){
//    case 1:Serial.println("现在进入了 1 号执行模式");break;
//    case 2:Serial.println("现在进入了 2 号执行模式");break;
//    case 3:Serial.println("现在进入了 3 号执行模式");break;
//    case 4:Serial.println("现在进入了 4 号执行模式");break;
//    default: Serial.println("程序异常");break;
//  }
}
