void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
for(int x=1; x<=9; x++){
  for(int y=1; y<=9; y++){
    Serial.print(String(x) + "*" + String(y) + "=" + String(x*y) + "\t");
  }
  Serial.println();
}
delay(500);
}
