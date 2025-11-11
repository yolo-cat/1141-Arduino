// 20231115: 增加多次測量取平均值的功能，以過濾噪點，提高測距穩定性。
// 20231116: 將距離計算變更為浮點數運算，以提高精度。
// 20231118: 改用整數運算並加入平滑濾波，優化距離顯示體驗。
// 20231119: 移除 pulseIn() 函數的自訂超時處理。
// 20231120: 將數字距離顯示改為視覺化長條圖。
// 20231121: 同時顯示數值距離與視覺化長條圖。
// 20231122: 優化反應速度，以非阻塞式 millis() 取代 delay() 並調整濾波參數。

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

// 降低平滑因子以加快反應速度
const int SMOOTHING_FACTOR = 3;
long smoothedDistance = -1;
const int MAX_DISTANCE_FOR_BAR = 150;

// 非阻塞式蜂鳴器控制變數
unsigned long lastBuzzerToggleTime = 0;
bool buzzerState = LOW;

// 定義自訂字元：全黑方塊
byte block[8] = {
  0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
};

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
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
  // --- 測距與濾波 ---
  // 減少採樣次數以縮短測量總時間
  const int numReadings = 3;
  long totalDistance = 0;
  for (int i = 0; i < numReadings; i++) {
    totalDistance += takeSingleReading();
    delay(5); // 每次採樣間的短暫延遲
  }
  long averageDistance = totalDistance / numReadings;
  if (smoothedDistance == -1) {
    smoothedDistance = averageDistance;
  } else {
    smoothedDistance = (averageDistance + (SMOOTHING_FACTOR - 1) * smoothedDistance) / SMOOTHING_FACTOR;
  }
  long distance = smoothedDistance;

  // --- 更新 LCD 顯示 ---
  char buffer[17];
  sprintf(buffer, "Dist:%5ld cm", distance);
  lcd.setCursor(0, 0);
  lcd.print(buffer);

  int barLength = map(distance, 0, MAX_DISTANCE_FOR_BAR, 0, 16);
  barLength = constrain(barLength, 0, 16);
  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.write(i < barLength ? (uint8_t)0 : ' ');
  }

  // --- 非阻塞式蜂鳴器控制 ---
  unsigned long currentMillis = millis();
  if (distance < 33) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerState = HIGH;
  } else if (distance >= 33 && distance < 66) {
    if (currentMillis - lastBuzzerToggleTime >= 125) { // 急促音間隔
      lastBuzzerToggleTime = currentMillis;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else if (distance >= 66 && distance < 99) {
    if (currentMillis - lastBuzzerToggleTime >= 250) { // 舒緩音間隔
      lastBuzzerToggleTime = currentMillis;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = LOW;
  }
}