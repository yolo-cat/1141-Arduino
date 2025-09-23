void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  char command = Serial.read();

      int n = Serial.parseInt();
      for (int i = 0; i < n; i++) {
          digitalWrite(BUILTIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
          delay(500);                       // wait for a second
          digitalWrite(BUILTIN_LED, LOW);    // turn the LED off by making the voltage LOW
          delay(500);                       // wait for a second
          digitalWrite(4, HIGH);
          delay(500);
          digitalWrite(4, LOW);
          delay(500);
  }
}


//   else if(command == `N`){
//       // Serial.println("LED ON N times");
//       // Serial.println("Enter a number:");
//       // while (Serial.available() == 0) {
//       //   // wait for user input
//       // }
//       int n = Serial.parseInt();
//       // Serial.print("Blinking LED ");
//       // Serial.print(n);
//       // Serial.println(" times");
//       for (int i = 0; i < n; i++) {
//         digitalWrite(4, HIGH);
//         delay(500);
//         digitalWrite(4, LOW);
//         delay(500);
//       }
//   }

// switch(command){
//     case 'O':
//       Serial.println("LED ON");
//       digitalWrite(4, HIGH);
//       break;
//     case 'X':
//         Serial.println("LED OFF");
//         digitalWrite(4, LOW);
//         break;
// //     輸入數字，x就亮x次，使用 Serial.parseInt()
//     case 'N':
//         // Serial.println("LED ON N times");
//         // Serial.println("Enter a number:");
//         // while (Serial.available() == 0) {
//         //   // wait for user input
//         // }
//         int n = Serial.parseInt();
//         // Serial.print("Blinking LED ");
//         // Serial.print(n);
//         // Serial.println(" times");
//         for (int i = 0; i < n; i++) {
//           digitalWrite(4, HIGH);
//           delay(500);
//           digitalWrite(4, LOW);
//           delay(500);
//         }
//         break;
//     }
// }
