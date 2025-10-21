// ESP32 三顆 LED 二進位顯示計數器與按鍵控制
// 腳位定義
const int buttonPin = 23;
// 將 LED 改接到可輸出的 GPIO（避開輸入專用的 GPIO34-39）
const int ledPins[] = {25, 26, 27}; // LSB到MSB，對應二進位的第0、1、2位

// 變數定義
int counter = 0;
int buttonState;
int lastButtonState = HIGH;

// 防彈跳與長按處理
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long holdStartTime = 0;
unsigned long nextIncrementTime = 0;
const unsigned long holdDelay = 500;

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 LED Counter Started");
  
  // 設定按鍵腳位（使用內建上拉電阻）
  pinMode(buttonPin, INPUT_PULLUP);
  
  // 設定 LED 腳位為輸出並初始化為 LOW
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  // 顯示初始狀態
  updateLeds();
  Serial.print("Initial counter: ");
  Serial.println(counter);
}

void loop() {
  // 讀取按鍵狀態
  int reading = digitalRead(buttonPin);
  
  // 檢查按鍵狀態是否改變（防彈跳處理）
  if (reading != lastButtonState) {
    // 重置防彈跳計時器
    lastDebounceTime = millis();
    holdStartTime = 0; // 狀態改變時重置長按計時器
  }
  
  // 防彈跳延遲後處理
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 確認按鍵狀態確實改變
    if (reading != buttonState) {
      buttonState = reading;
      
      // 檢測按鍵按下（HIGH->LOW，因為使用 INPUT_PULLUP）
      if (buttonState == LOW) {
        incrementCounter();
        holdStartTime = millis();
        nextIncrementTime = holdStartTime + holdDelay;
      }
    }
  }
  
  // 處理按鍵長按連續遞增
  if (buttonState == LOW && holdStartTime > 0) {
    if (millis() >= nextIncrementTime) {
      incrementCounter();
      nextIncrementTime = millis() + holdDelay;
    }
  }
  
  // 儲存按鍵狀態供下次比較
  lastButtonState = reading;
}

void incrementCounter() {
  counter++;
  if (counter > 7) {
    counter = 0; // 0-7 循環（3位元二進位）
  }
  
  Serial.print("Counter: ");
  Serial.print(counter);
  Serial.print(" (Binary: ");
  Serial.print(counter, BIN);
  Serial.println(")");
  
  updateLeds();
}

void updateLeds() {
  // 根據 counter 值更新三顆 LED 的狀態
  for (int i = 0; i < 3; i++) {
    // 使用位元運算檢查第 i 位是否為 1
    bool ledState = (counter >> i) & 1;
    digitalWrite(ledPins[i], ledState);
    
    // 可選：添加小延遲讓 LED 變化更明顯
    // delay(50);
  }
}
