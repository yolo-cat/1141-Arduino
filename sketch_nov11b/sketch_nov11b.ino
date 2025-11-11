// 光敏電阻的阻值顯示在LCD第一行
// 如果光敏電阻的值小於一定數值時，第二行顯示"Night";反之顯示"Morning"
// 連動 LED 燈，Night 亮燈，Morning 熄燈

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 硬體接腳定義
#define I2C_SDA_PIN         26
#define I2C_SCL_PIN         25

// LCD 設定
#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

// LCD I2C 設定 (位址 0x27, 16x2 顯示)
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 光敏電阻腳位
#define LIGHT_SENSOR_PIN 13

// RGB LED 腳位定義
#define RED_PIN   4
#define GREEN_PIN 0
#define BLUE_PIN  16

// 閾值設定 (根據實際環境調整)
#define NIGHT_THRESHOLD 2000

void setup() {
  // 初始化序列埠
  Serial.begin(115200);

  // 初始化光敏電阻腳位
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // 初始化 RGB LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // 關閉所有 LED
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  // 初始化 I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // 初始化 LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // 顯示啟動訊息
  lcd.setCursor(0, 0);
  lcd.print("Light Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  Serial.println("系統初始化完成");
}

void loop() {
  // 讀取光敏電阻的值
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  // 顯示在序列埠監控視窗
  Serial.print("Light Value: ");
  Serial.println(lightValue);

  // 在 LCD 第一行顯示光敏電阻的值
  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(lightValue);
  lcd.print("    "); // 清除舊數據

  // 判斷是白天還是夜晚
  lcd.setCursor(0, 1);
  if (lightValue < NIGHT_THRESHOLD) {
    // 夜晚：顯示 "Night" 並開燈 (紅色)
    lcd.print("Night          ");
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    Serial.println("狀態: Night (RED LED ON)");
  } else {
    // 白天：顯示 "Morning" 並關燈
    lcd.print("Morning        ");
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    Serial.println("狀態: Morning (LED OFF)");
  }

  // 延遲一段時間再重新讀取
  delay(500);
}
