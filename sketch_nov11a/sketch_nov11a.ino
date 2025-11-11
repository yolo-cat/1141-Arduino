// 20231115: 增加多次測量取平均值的功能，以過濾噪點，提高測距穩定性。
// 20231116: 將距離計算變更為浮點數運算，以提高精度。
// 20231117: 處理 pulseIn() 函數超時情況，避免將無效讀值誤判為 0 公分。
// 20231118: 改用整數運算並加入平滑濾波，優化距離顯示體驗。
// 20231119: 移除自訂的 pulseIn() 超時處理，恢復預設行為。

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

// 平滑濾波因子，值越大，顯示越平滑
const int SMOOTHING_FACTOR = 10;
// 用於存放平滑後距離值的變數，-1 代表尚未初始化
long smoothedDistance = -1;

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
}

// 執行一次超音波測距並回傳距離（公分）
long takeSingleReading() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  // 使用 pulseIn 的預設超時行為（1秒）
  long duration = pulseIn(EchoPin, HIGH);

  // 使用整數運算 (duration * 17 / 1000) 替代 (duration * 0.034 / 2)
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
    // 如果是第一次讀取，直接賦值
    smoothedDistance = averageDistance;
  } else {
    // 應用平滑濾波
    smoothedDistance = (averageDistance + (SMOOTHING_FACTOR - 1) * smoothedDistance) / SMOOTHING_FACTOR;
  }
  
  long distance = smoothedDistance;

  // 顯示距離到 LCD
  lcd.setCursor(0, 1);
  lcd.print("      ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  // 根據距離區間控制蜂鳴器
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