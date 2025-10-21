// 定義超音波測距儀接腳
int TrigPin = 17;
int EchoPin = 16;

// 定義RGB燈接腳
int RPin = 32;
int GPin = 33;
int BPin = 25;

void setup() {
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  // 設定RGB燈接腳為輸出
  pinMode(RPin, OUTPUT);
  pinMode(GPin, OUTPUT);
  pinMode(BPin, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // 設置距離公式
  unsigned long distance = ping() / 58;

  Serial.print(distance);
  Serial.println("cm");

  // 假設我們關注的距離範圍是 5 到 50 公分
  // 將距離限制在 5 到 50 之間，避免極端值影響顏色
  int constrained_distance = constrain(distance, 5, 50);

  // 將距離映射到顏色值 (0-255)
  // 距離越近(5cm)，紅色越亮(255)；距離越遠(50cm)，紅色越暗(0)
  int redValue = map(constrained_distance, 5, 50, 255, 0);
  // 距離越近(5cm)，綠色越暗(0)；距離越遠(50cm)，綠色越亮(255)
  int greenValue = map(constrained_distance, 5, 50, 0, 255);

  // 寫入顏色值到RGB燈
  // 在 ESP32 上，analogWrite 實際上是使用 LEDC (LED Control) 功能來產生 PWM 波
  analogWrite(RPin, redValue);
  analogWrite(GPin, greenValue);
  analogWrite(BPin, 0); // 藍色保持關閉

  delay(100);
}

unsigned long ping() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  return pulseIn(EchoPin, HIGH);
}
