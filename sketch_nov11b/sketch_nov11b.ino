// 光敏電阻的阻值顯示在LCD第一行
// 如果光敏電阻的值小於一定數值時，第二行顯示"Night";反之顯示"Morning"
// 連動 LED 燈，Night 亮燈，Morning 熄燈
// 加入 PWM 燈光漸變功能

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
#define BRIGHT_THRESHOLD 3000

// 漸變速度 (越小越快)
#define FADE_SPEED 5

// 目前和目標亮度
int currentBrightness = 0;
int targetBrightness = 0;

void setup() {
  // 初始化序列埠
  Serial.begin(115200);

  // 初始化光敏電阻腳位
  pinMode(LIGHT_SENSOR_PIN, INPUT);

  // 初始化 RGB LED 腳位為輸出
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // 關閉所有 LED
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);

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
  Serial.println("PWM 漸變功能已啟用");
}

void loop() {
  // 讀取光敏電阻的值
  int lightValue = analogRead(LIGHT_SENSOR_PIN);

  // 顯示在序列埠監控視窗
  Serial.print("Light Value: ");
  Serial.print(lightValue);
  Serial.print(" | Brightness: ");
  Serial.println(currentBrightness);

  // 在 LCD 第一行顯示光敏電阻的值
  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(lightValue);
  lcd.print("    "); // 清除舊數據

  // 判斷是白天還是夜晚，並設定目標亮度
  lcd.setCursor(0, 1);
  if (lightValue < NIGHT_THRESHOLD) {
    // 夜晚：顯示 "Night" 並設定 LED 全亮
    lcd.print("Night          ");
    targetBrightness = 255;
    Serial.print("狀態: Night | ");
  } else if (lightValue > BRIGHT_THRESHOLD) {
    // 白天：顯示 "Morning" 並關燈
    lcd.print("Morning        ");
    targetBrightness = 0;
    Serial.print("狀態: Morning | ");
  } else {
    // 黃昏/黎明：顯示 "Twilight" 並設定中等亮度
    lcd.print("Twilight       ");
    // 根據光線值計算亮度 (線性映射)
    targetBrightness = map(lightValue, NIGHT_THRESHOLD, BRIGHT_THRESHOLD, 255, 0);
    targetBrightness = constrain(targetBrightness, 0, 255);
    Serial.print("狀態: Twilight | ");
  }

  // 平滑漸變到目標亮度
  if (currentBrightness < targetBrightness) {
    currentBrightness += FADE_SPEED;
    if (currentBrightness > targetBrightness) {
      currentBrightness = targetBrightness;
    }
  } else if (currentBrightness > targetBrightness) {
    currentBrightness -= FADE_SPEED;
    if (currentBrightness < targetBrightness) {
      currentBrightness = targetBrightness;
    }
  }

  // 設定紅色 LED 的 PWM 值 (漸變效果)
  analogWrite(RED_PIN, currentBrightness);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);

  // 延遲一段時間再重新讀取
  delay(50);
}
