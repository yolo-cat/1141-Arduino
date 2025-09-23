void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(16, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
    bool p16 = digitalRead(16);

    Serial.println(p16);
    if  (p16) {
      digitalWrite(4, LOW);
    }
    else {
      digitalWrite(4, HIGH);
    }
    delay(100);
        }
