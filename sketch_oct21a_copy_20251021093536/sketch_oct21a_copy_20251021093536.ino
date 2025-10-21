#define PIN_R 32
#define PIN_G 33
#define PIN_B 25
#define BUTTON_PIN 4

int lastButtonState = HIGH; // 上一次按鍵狀態

int valR = 0; // 紅色亮度
int valG = 0; // 綠色亮度
int valB = 0; // 藍色亮度

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(0)); // 讀取一個未連接的類比腳位來產生亂數種子
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // 偵測按鍵由 HIGH 變 LOW（按下瞬間）
  if (lastButtonState == HIGH && buttonState == LOW) {
    // 產生隨機的 RGB 顏色值
    valR = random(256); // 0-255
    valG = random(256); // 0-255
    valB = random(256); // 0-255
    delay(50); // 簡單防彈跳
  }
  lastButtonState = buttonState;

  // 同時輸出三色亮度
  analogWrite(PIN_R, valR);
  analogWrite(PIN_G, valG);
  analogWrite(PIN_B, valB);
}