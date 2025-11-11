// 初始化 DHT
#include <DHT.h>
#define DHTPIN 4     // DHT 传感器连接到数字引脚 4
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// RGB LED 腳位定義
#define RED_PIN   32
#define GREEN_PIN 33
#define BLUE_PIN  25

void setup() {
  // 启动串口通信
  Serial.begin(115200);
  Serial.println("DHTxx + RGB test!");
  dht.begin();

  // 設定 RGB 腳位為輸出
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // 初始狀態全部關閉
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");

  // RGB 控制邏輯
  if (t >= 30 || h >= 90) {
    // 紅燈
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  } else if (t < 30 && h >= 70 && h < 90) {
    // 藍燈
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  } else if (t < 30 && h < 70) {
    // 綠燈
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  } else {
    // 預設全部關閉
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }

  delay(2000);
}
