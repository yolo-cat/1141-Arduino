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

void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

// Virtual Pin
    BLYNK_WRITE(V4) {
        int pinValue = param.asInt();
        digitalWrite(4, pinValue);
        Serial.print("V4 Slider value: ");
        Serial.println(pinValue);
    }

void loop() {
    Blynk.run();
}
