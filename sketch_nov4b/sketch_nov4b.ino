#include <Arduino.h>

#define BUZZER_PIN 14

// Mario main theme melody (片段)
int melody[] = {
  659, 659, 0, 659, 0, 523, 659, 0, 784, // 開頭
  0, 392, 523, 0, 392, 0, 330,           // 第二段
  440, 494, 466, 440, 392, 659, 784, 880 // 結尾
};

// 每個音符的長度 (ms)
int noteDurations[] = {
  150, 150, 150, 150, 150, 150, 150, 150, 300,
  150, 150, 150, 150, 150, 150, 150,
  150, 150, 150, 150, 150, 150, 150, 300
};

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < sizeof(melody)/sizeof(int); i++) {
    if (melody[i] == 0) {
      // 休止符
      noTone(BUZZER_PIN);
    } else {
      tone(BUZZER_PIN, melody[i], noteDurations[i]);
    }
    delay(noteDurations[i] * 1.3); // 讓音符分明
  }
  delay(2000); // 播完一遍後暫停 2 秒
}
