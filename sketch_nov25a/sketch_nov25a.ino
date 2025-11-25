/*
 * 版本更新註釋:
 * 2024-11-25 v1: 修復 GPIO 4 輸入輸出衝突,實體按鈕改用 GPIO 17,實現虛擬與實體按鈕雙向控制 LED 並同步狀態到 V5 Label
 * 2024-11-25 v2: 修正按鈕邏輯從切換模式改為按住/放開模式,按住時 LED 亮顯示 Button Down,放開時 LED 滅顯示 Button Up
 * 2024-11-25 v3: 加入計數器強制刷新 Blynk Label 顯示,解決 Label 元件不更新問題
 */

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6IoglQkK7"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "X12hSrsYVoGdAsrW03_V8ys5YebyvefK"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// WiFi authentication details
char ssid[] = "JoXP10vi";
char password[] = "0919091821";

// GPIO pins
const int LED_PIN = 4;
const int BUTTON_PIN = 17;

// LED state
bool ledState = LOW;
bool lastButtonState = HIGH;
int updateCounter = 0;

// Virtual Pin V4 - 虛擬按鈕控制 LED
BLYNK_WRITE(V4) {
    int pinValue = param.asInt();
    ledState = pinValue;
    digitalWrite(LED_PIN, ledState);

    // 同步狀態到 V5 Label (加入計數器強制刷新)
    updateCounter++;
    if(ledState == HIGH) {
        Blynk.virtualWrite(V5, String("Button Down #") + String(updateCounter));
    } else {
        Blynk.virtualWrite(V5, String("Button Up #") + String(updateCounter));
    }

    Serial.print("V4 Button value: ");
    Serial.println(pinValue);
}

void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(LED_PIN, LOW);

    // 初始化 V5 顯示
    Blynk.virtualWrite(V5, "Button Up #0");
}

void loop() {
    Blynk.run();
    int button_status = digitalRead(BUTTON_PIN);

    // 按鈕狀態改變時更新
    if(button_status != lastButtonState) {

        updateCounter++;

        if(button_status == LOW) {
            // 按鈕被按下
            ledState = HIGH;
            digitalWrite(LED_PIN, HIGH);

            // 同步到 Blynk V4 虛擬按鈕
            Blynk.virtualWrite(V4, HIGH);

            // 更新 V5 Label 顯示 (加入計數器強制刷新)
            Blynk.virtualWrite(V5, String("Button Down #") + String(updateCounter));

            Serial.println("Physical button pressed, LED ON");

        } else {
            // 按鈕被放開
            ledState = LOW;
            digitalWrite(LED_PIN, LOW);

            // 同步到 Blynk V4 虛擬按鈕
            Blynk.virtualWrite(V4, LOW);

            // 更新 V5 Label 顯示 (加入計數器強制刷新)
            Blynk.virtualWrite(V5, String("Button Up #") + String(updateCounter));

            Serial.println("Physical button released, LED OFF");
        }

        lastButtonState = button_status;
        delay(50); // 消除按鈕彈跳
    }

    delay(10);
}
