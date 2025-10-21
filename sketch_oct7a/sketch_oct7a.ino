// 軟體 PWM 範例：不使用 ledcSetup，使用 digitalWrite + micros()
// X/Y 類比讀取 → 轉為 0..255 (digital 255) → 控制兩顆 LED 亮度

#define SW_PIN    4    // 搖桿開關 (INPUT_PULLUP)
#define LED1_PIN  0    // LED1 (建議改為非 boot pin, 若有問題請改用其他腳)
#define LED2_PIN  2    // LED2
#define VRX_PIN  13    // X 軸 ADC
#define VRY_PIN  35    // Y 軸 ADC（可改回 9，但 9 在部分板子可能受限）

// 軟體 PWM 參數
const unsigned long PWM_PERIOD_US = 2000UL; // PWM 週期：2000us = 500Hz
const unsigned long SENSOR_SAMPLE_MS = 50UL; // 每多少毫秒更新一次類比讀值

// 內部變數
volatile int duty1_us = 0;
volatile int duty2_us = 0;
unsigned long lastSampleMicros = 0;
unsigned long lastSampleMillis = 0;

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  Serial.begin(115200);
  lastSampleMicros = micros();
  lastSampleMillis = millis();
}

void sampleSensorsAndComputeDuty() {
  // 讀取 raw 類比值
  int xRaw = analogRead(VRX_PIN);
  int yRaw = analogRead(VRY_PIN);

  // 根據平台選 ADC 最大值（ESP32 常見 4095；AVR 為 1023）
  #if defined(ESP32)
    const int ADC_MAX = 4095;
  #else
    const int ADC_MAX = 1023;
  #endif

  // 轉換到 0..255（digital 255）
  int b1 = map(xRaw, 0, ADC_MAX, 0, 255);
  int b2 = map(yRaw, 0, ADC_MAX, 0, 255);
  b1 = constrain(b1, 0, 255);
  b2 = constrain(b2, 0, 255);

  // 換算為微秒佔空時間（0..PWM_PERIOD_US）
  duty1_us = (int)((uint32_t)b1 * PWM_PERIOD_US / 255UL);
  duty2_us = (int)((uint32_t)b2 * PWM_PERIOD_US / 255UL);

  // 印出 debug（可註解以節省序列埠）
  Serial.print("Xraw:"); Serial.print(xRaw);
  Serial.print(" X->"); Serial.print(b1);
  Serial.print("  Yraw:"); Serial.print(yRaw);
  Serial.print(" Y->"); Serial.print(b2);
  Serial.print("  duty1_us:"); Serial.print(duty1_us);
  Serial.print(" duty2_us:"); Serial.println(duty2_us);
}

void loop() {
  unsigned long nowMicros = micros();

  // 週期性更新類比讀值與佔空時間（不必每個 PWM 週期都重新讀 ADC）
  if (millis() - lastSampleMillis >= SENSOR_SAMPLE_MS) {
    sampleSensorsAndComputeDuty();
    lastSampleMillis = millis();
  }

  int swState = digitalRead(SW_PIN); // 1 = 沒按, 0 = 按下

  if (swState == LOW) {
    // 按下時兩顆燈都滅（直接 LOW）
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    // 等待下一迴圈（仍需快速迴圈保持反應）
    return;
  }

  // 依照當前時間決定 LED 是否該亮（軟體 PWM）
  unsigned long phase = nowMicros % PWM_PERIOD_US;

  // LED1
  if (phase < (unsigned long)duty1_us) {
    digitalWrite(LED1_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
  }

  // LED2
  if (phase < (unsigned long)duty2_us) {
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }

  // 注意：不要在這裡加入長時間 delay()，否則會干擾 PWM 的形狀
  // 可讓 loop 自由迴圈以維持 PWM 穩定
}
