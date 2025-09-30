void setup() {
  Serial.begin(115200);
  pinMode(13, INPUT);
}

void loop() {
    int val = analogRead(13);
    Serial.println(val);
    delay(10);
    }
