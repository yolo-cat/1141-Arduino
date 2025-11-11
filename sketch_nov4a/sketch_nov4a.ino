#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 26 /* Define the SDA pin here */
#define SCL_PIN 25 /* Define the SCL pin here */

// 初始化 DHT
#include <DHT.h>
#define DHTPIN 4     // DHT 传感器连接到数字引脚 4
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2); /* set the LCD address to 0x27 for a 16 chars and 2 line display */

void setup() {
    // 启动串口通信
    Serial.begin(115200);
    Serial.println("DHTxx + RGB test!");
    dht.begin();
    Wire.begin(SDA_PIN, SCL_PIN); /* Initialize I2C communication */
    lcd.init();      /* initialize the lcd */
    lcd.backlight();
    lcd.clear();
}

void loop() {
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1); // 顯示一位小數
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("Humi: ");
    lcd.print(h, 1); // 顯示一位小數
    lcd.print(" %");

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

    delay(1000);
}
