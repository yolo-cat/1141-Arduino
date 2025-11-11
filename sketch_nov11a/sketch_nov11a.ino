// ============================================================================
// 超音波測距系統 - NodeMCU-32S
// 簡化版本：基本測距與蜂鳴器聯動
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 硬體接腳定義
#define I2C_SDA_PIN         26
#define I2C_SCL_PIN         25
#define ULTRASONIC_TRIG_PIN 17
#define ULTRASONIC_ECHO_PIN 16
#define BUZZER_PIN          14

// LCD 設定
#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

// 蜂鳴器距離閾值 (cm)
#define DISTANCE_NEAR 33
#define DISTANCE_MID  66
#define DISTANCE_FAR  99

// 長條圖設定
#define MAX_DISTANCE  200  // 最大顯示距離 (cm)
#define BAR_MAX_WIDTH 16   // 長條圖最大寬度 (LCD 寬度)

// 蜂鳴器間隔 (ms)
#define BUZZER_INTERVAL_MID 125
#define BUZZER_INTERVAL_FAR 250

// 全域物件
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 全域變數
unsigned long lastBuzzerTime = 0;
bool buzzerState = LOW;

void setup() {
  // 初始化 I2C 與 LCD
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  lcd.init();
  lcd.backlight();

  // 初始化超音波感測器
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // 初始化蜂鳴器
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  long distance = measureDistance();
  displayDistance(distance);
  controlBuzzer(distance);
}

// 測量距離
long measureDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  return (duration * 17) / 1000;
}

// 顯示距離
void displayDistance(long distance) {
  // 第一行：顯示距離數字
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm    ");

  // 第二行：顯示長條圖
  lcd.setCursor(0, 1);

  // 計算長條圖長度 (距離越近,長條越長)
  int barLength = map(distance, 0, MAX_DISTANCE, BAR_MAX_WIDTH, 0);
  barLength = constrain(barLength, 0, BAR_MAX_WIDTH);

  // 繪製黑色長條 (使用 0xFF 字元作為實心方塊)
  for (int i = 0; i < BAR_MAX_WIDTH; i++) {
    if (i < barLength) {
      lcd.write(0xFF);  // 黑色方塊
    } else {
      lcd.print(" ");   // 空白
    }
  }
}

// 控制蜂鳴器
void controlBuzzer(long distance) {
  unsigned long currentTime = millis();

  if (distance < DISTANCE_NEAR) {
    // 近距離：持續響
    digitalWrite(BUZZER_PIN, HIGH);

  } else if (distance < DISTANCE_MID) {
    // 中距離：快速閃爍
    if (currentTime - lastBuzzerTime >= BUZZER_INTERVAL_MID) {
      lastBuzzerTime = currentTime;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }

  } else if (distance < DISTANCE_FAR) {
    // 遠距離：慢速閃爍
    if (currentTime - lastBuzzerTime >= BUZZER_INTERVAL_FAR) {
      lastBuzzerTime = currentTime;
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }

  } else {
    // 超出範圍：關閉
    digitalWrite(BUZZER_PIN, LOW);
  }
}
