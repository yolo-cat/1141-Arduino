void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  char command = Serial.read();

  if(command == 'N'){
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
