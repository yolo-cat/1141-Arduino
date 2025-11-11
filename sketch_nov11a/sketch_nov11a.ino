```// 20231115: 增加多次測量取平均值的功能，以過濾噪點，提高測距穩定性。
// 20231115: 將距離計算變數改為 float 型別，以修復整數運算造成的精度損失問題。

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

void setup() {
  // 設定 I2C 腳位
  Wire.begin(SDA_PIN, SCL_PIN);

  // 初始化 LCD
  lcd.init();
  lcd.backlight();

  // 初始化超音波腳位
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // 初始化蜂鳴器腳位
  pinMode(BUZZER_PIN, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Distance:");
}

// 執行一次超音波測距並回傳距離（公分）
float takeSingleReading() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  long duration = pulseIn(EchoPin, HIGH);
  // 使用浮點數運算以保留小數精度
  return duration * 0.034 / 2.0;
}

void loop() {
  const int numReadings = 5;
  float totalDistance = 0;

  // 進行多次測量並加總
  for (int i = 0; i < numReadings; i++) {
    totalDistance += takeSingleReading();
    delay(10); // 每次測量間短暫延遲
  }

  // 計算平均距離
  float distance = totalDistance / numReadings;

  // 顯示距離到 LCD
  lcd.setCursor(0, 1);
  lcd.print("      "); // 清除舊資料
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  // 根據距離區間控制蜂鳴器
  if (distance < 33) {
    // 距離小於 33 公分：持續長音
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100); // 短暫延遲以保持迴圈穩定
  } else if (distance >= 33 && distance < 66) {
    // 33cm - 66cm：100ms 急促間隔音
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  } else if (distance >= 66 && distance < 99) {
    // 66cm - 99cm：200ms 舒緩間隔音
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  } else {
    // 大於等於 99cm：不發音
    digitalWrite(BUZZER_PIN, LOW);
    delay(500); // 保持與舒緩間隔音相似的更新率
  }
}
