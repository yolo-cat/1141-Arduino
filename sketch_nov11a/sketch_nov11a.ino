// 20231115: 增加多次測量取平均值的功能，以過濾噪點，提高測距穩定性。
// 20231116: 將距離計算變更為浮點數運算，以提高精度。
// 20231118: 改用整數運算並加入平滑濾波，優化距離顯示體驗。
// 20231119: 移除 pulseIn() 函數的自訂超時處理。
// 20231120: 將數字距離顯示改為視覺化長條圖。
// 20231121: 同時顯示數值距離與視覺化長條圖。

// 引入 I2C 和 LCD 函式庫
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SDA_PIN 26 /* Define the SDA pin here */
#define SCL_PIN 25 /* Define the SCL pin here */

// 定義超音波測距儀接腳
int TrigPin = 17;
int EchoPin = 16;

#include <Arduino.h>
#define BUZZER_PIN 14

// LCD 物件 (根據你的 LCD I2C 位址，常見為 0x27 或 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 平滑濾波因子
const int SMOOTHING_FACTOR = 10;
// 用於存放平滑後距離值的變數
long smoothedDistance = -1;
// 用於長條圖顯示的最大距離 (公分)
const int MAX_DISTANCE_FOR_BAR = 150;

// 定義自訂字元：全黑方塊
byte block[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // 創建自訂字元，索引為 0
  lcd.createChar(0, block);
}

// 執行一次超音波測距並回傳距離（公分）
long takeSingleReading() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  long duration = pulseIn(EchoPin, HIGH);
  return (duration * 17) / 1000;
}

void loop() {
  const int numReadings = 5;
  long totalDistance = 0;

  for (int i = 0; i < numReadings; i++) {
    totalDistance += takeSingleReading();
    delay(10);
  }

  long averageDistance = totalDistance / numReadings;

  if (smoothedDistance == -1) {
    smoothedDistance = averageDistance;
  } else {
    smoothedDistance = (averageDistance + (SMOOTHING_FACTOR - 1) * smoothedDistance) / SMOOTHING_FACTOR;
  }
  long distance = smoothedDistance;

  // --- 更新 LCD 顯示 ---

  // 第一行：顯示數值距離
  char buffer[17];
  sprintf(buffer, "Distance:%4ld cm", distance); // 將字串格式化為 16 字元
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  // 第二行：顯示長條圖
  int barLength = map(distance, 0, MAX_DISTANCE_FOR_BAR, 0, 16);
  barLength = constrain(barLength, 0, 16);

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    if (i < barLength) {
      lcd.write((uint8_t)0); // 寫入自訂方塊字元
    } else {
      lcd.print(" "); // 清除長條圖剩餘部分
    }
  }

  // --- 根據距離區間控制蜂鳴器 ---
  if (distance < 33) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
  } else if (distance >= 33 && distance < 66) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(125);
    digitalWrite(BUZZER_PIN, LOW);
    delay(125);
  } else if (distance >= 66 && distance < 99) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(250);
    digitalWrite(BUZZER_PIN, LOW);
    delay(250);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
  }
}
