#define PIN_R 32
#define PIN_G 33
#define PIN_B 25
#define BUTTON_PIN 4
#define POT_PIN 36 // GPIO36 (VP, A0)

int colorIndex = 0;         // 0:紅, 1:綠, 2:藍
int lastButtonState = HIGH; // 上一次按鍵狀態

int valR = 255; // 紅色亮度
int valG = 255; // 綠色亮度
int valB = 255; // 藍色亮度

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  // 讀取可變電阻，轉換為 0~255
  int potValue = analogRead(POT_PIN); // 0~4095
  int brightness = map(potValue, 0, 4095, 0, 255);

  // 偵測按鍵由 HIGH 變 LOW（按下瞬間）
  if (lastButtonState == HIGH && buttonState == LOW) {
    colorIndex = (colorIndex + 1) % 3; // 只循環紅綠藍
    delay(50); // 簡單防彈跳
  }
  lastButtonState = buttonState;

  // 只有目前選中的顏色會根據旋鈕調整亮度
  switch (colorIndex) {
    case 0: // 紅
      if (abs(brightness - valR) > 2) valR = brightness;
      break;
    case 1: // 綠
      if (abs(brightness - valG) > 2) valG = brightness;
      break;
    case 2: // 藍
      if (abs(brightness - valB) > 2) valB = brightness;
      break;
  }

  // 同時輸出三色亮度
  analogWrite(PIN_R, valR);
  analogWrite(PIN_G, valG);
  analogWrite(PIN_B, valB);
}
