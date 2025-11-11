#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 26 /* Define the SDA pin here */
#define SCL_PIN 25 /* Define the SCL pin here */

LiquidCrystal_I2C lcd(0x27, 16, 2); /* set the LCD address to 0x27 for a 16 chars and 2 line display */

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN); /* Initialize I2C communication */
  lcd.init();      /* initialize the lcd */
  lcd.backlight();
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Hello, From");
  lcd.setCursor(0, 1);
  lcd.print("Nodemcu!");

  delay(1000);
  lcd.clear();
}
