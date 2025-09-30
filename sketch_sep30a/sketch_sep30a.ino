// 定義腳位
const int buttonPin = 23;
// 將 LED 分配給腳位。假設 34 是最低有效位 (LSB)，36 是最高有效位 (MSB)。
const int ledPins[] = {34, 39, 36};

// 計數器變數
int counter = 0;

// 按鍵處理變數
int buttonState;             // 輸入腳位的目前讀取值
int lastButtonState = HIGH;  // 輸入腳位的先前讀取值

// a因為時間 (以毫秒為單位) 會很快變成一個比 int 所能儲存的數字還大的數字，所以下面的變數是 unsigned long。
unsigned long lastDebounceTime = 0;  // 上次輸出腳位切換的時間
unsigned long debounceDelay = 50;    // 防彈跳時間；如果輸出閃爍，請增加此值

unsigned long holdStartTime = 0;      // 首次按下按鍵的時間
unsigned long nextIncrementTime = 0;  // 長按期間下次應增加的時間
const unsigned long holdDelay = 500;  // 連續增加的延遲

void setup() {
  Serial.begin(115200);

  // 設定按鍵腳位
  pinMode(buttonPin, INPUT_PULLUP);

  // 設定 LED 腳位
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
  }

  // LED 初始狀態
  updateLeds();
}

void loop() {
  // 將開關狀態讀取到區域變數中：
  int reading = digitalRead(buttonPin);

  // 檢查您是否剛按下按鍵
  // (即輸入從 HIGH 變為 LOW)，並且您已經等待了足夠長的時間
  // 自上次按下以來，以忽略任何雜訊：

  // 如果開關因雜訊或按壓而改變：
  if (reading != lastButtonState) {
    // 重置防彈跳計時器
    lastDebounceTime = millis();
    holdStartTime = 0; // 如果狀態改變，重置長按計時器
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 無論讀取值是什麼，它都已經存在超過防彈跳
    // 延遲，所以將其視為實際的目前狀態：
    if (reading != buttonState) {
      buttonState = reading;

      // 如果剛按下按鍵
      if (buttonState == LOW) {
        incrementCounter();
        holdStartTime = millis();
        nextIncrementTime = holdStartTime + holdDelay;
      }
    }
  }

  // 處理按鍵長按以連續增加
  if (buttonState == LOW && holdStartTime > 0) {
    if (millis() >= nextIncrementTime) {
      incrementCounter();
      nextIncrementTime = millis() + holdDelay; // 安排下次增加
    }
  }

  // 儲存讀取值。下次迴圈時，它將是 lastButtonState：
  lastButtonState = reading;
}

void incrementCounter() {
  counter++;
  if (counter > 7) {
    counter = 0;
  }
  Serial.println(counter);
  updateLeds();
}

void updateLeds() {
  // 迭代每個 LED
  for (int i = 0; i < 3; i++) {
    // 使用位元 AND 和右移來取得每個位元的狀態
    // 如果第 i 位是 1，則 (counter >> i) & 1 將為 1，否則為 0。
    if ((counter >> i) & 1) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}
