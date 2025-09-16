void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  char command = Serial.read();

switch(command){
    case 'O':
      Serial.println("LED ON");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      break;
    case 'X':
        Serial.println("LED OFF");
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        break;
//     輸入數字，x就亮x次，使用 Serial.parseInt()
    case 'N':
        // Serial.println("LED ON N times");
        // Serial.println("Enter a number:");
        // while (Serial.available() == 0) {
        //   // wait for user input
        // }
        int n = Serial.parseInt();
        // Serial.print("Blinking LED ");
        // Serial.print(n);
        // Serial.println(" times");
        for (int i = 0; i < n; i++) {
          digitalWrite(LED_BUILTIN, HIGH);
          delay(500);
          digitalWrite(LED_BUILTIN, LOW);
          delay(500);
        }
        break;
    }
}
