// ============================================================================
// 超音波測距系統 - NodeMCU-32S
// 簡化版本:基本測距與蜂鳴器聯動
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

// 全域變數
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 自訂字元 - 長條圖用
byte barFull[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte barEmpty[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// ============================================================================
// 初始化函式
// ============================================================================
void setup() {
  // 序列埠初始化
  Serial.begin(115200);
  Serial.println("超音波測距系統啟動中...");

  // I2C 初始化
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // LCD 初始化
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // 建立自訂字元
  lcd.createChar(0, barFull);
  lcd.createChar(1, barEmpty);

  // 顯示啟動訊息
  lcd.setCursor(0, 0);
  lcd.print("Distance Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // 超音波感測器接腳設定
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // 蜂鳴器接腳設定
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("系統初始化完成!");
}

// ============================================================================
// 主迴圈
// ============================================================================
void loop() {
  // 讀取距離
  float distance = measureDistance();

  // 顯示距離資訊
  displayDistance(distance);

  // 根據距離控制蜂鳴器
  controlBuzzer(distance);

  // 顯示長條圖
  displayBarGraph(distance);

  // 序列埠輸出
  Serial.print("距離: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100);  // 更新頻率控制
}

// ============================================================================
// 超音波測距函式
// ============================================================================
float measureDistance() {
  // 發送 10us 的觸發脈衝
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  // 讀取回波時間 (微秒)
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH, 30000);  // 30ms 超時

  // 計算距離 (cm)
  // 音速 = 340 m/s = 0.034 cm/us
  // 距離 = (時間 × 音速) / 2
  float distance = duration * 0.034 / 2.0;

  // 距離有效性檢查
  if (distance == 0 || distance > MAX_DISTANCE) {
    distance = MAX_DISTANCE;  // 超出範圍
  }

  return distance;
}

// ============================================================================
// LCD 顯示距離
// ============================================================================
void displayDistance(float distance) {
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");

  if (distance >= MAX_DISTANCE) {
    lcd.print("---.- cm");
  } else {
    // 格式化顯示,保留一位小數
    char buffer[10];
    sprintf(buffer, "%5.1f", distance);
    lcd.print(buffer);
    lcd.print(" cm");
  }

  // 清除行尾可能的殘留字元
  lcd.print("  ");
}

// ============================================================================
// 蜂鳴器控制函式
// ============================================================================
void controlBuzzer(float distance) {
  static unsigned long lastBeepTime = 0;
  unsigned long currentTime = millis();
  unsigned long beepInterval = 0;

  // 根據距離決定蜂鳴間隔
  if (distance < DISTANCE_NEAR) {
    // 近距離:持續長音
    digitalWrite(BUZZER_PIN, HIGH);
    return;
  } else if (distance < DISTANCE_MID) {
    // 中距離:中速蜂鳴 (300ms 間隔)
    beepInterval = 300;
  } else if (distance < DISTANCE_FAR) {
    // 遠距離:慢速蜂鳴 (600ms 間隔)
    beepInterval = 600;
  } else {
    // 超遠距離:不蜂鳴
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // 間隔時間到達時切換蜂鳴器狀態
  if (currentTime - lastBeepTime >= beepInterval) {
    static bool buzzerState = false;
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
    lastBeepTime = currentTime;
  }
}

// ============================================================================
// 長條圖顯示函式
// ============================================================================
void displayBarGraph(float distance) {
  // 計算長條圖寬度 (距離越近,長條越長)
  int barWidth = 0;

  if (distance < MAX_DISTANCE) {
    // 反向映射:距離越近,長條越長
    barWidth = map(distance, 0, MAX_DISTANCE, BAR_MAX_WIDTH, 0);
    barWidth = constrain(barWidth, 0, BAR_MAX_WIDTH);
  }

  // 在第二行顯示長條圖
  lcd.setCursor(0, 1);

  // 繪製長條
  for (int i = 0; i < BAR_MAX_WIDTH; i++) {
    if (i < barWidth) {
      lcd.write(byte(0));  // 實心方塊
    } else {
      lcd.write(' ');      // 空白
    }
  }
}
