#include <BleConnectionStatus.h>
#include <BleMouse.h>
#include <M5Core2.h>

BleMouse bleMouse;
int prev_x = -1;
int prev_y = -1;
unsigned long previousMillis = 0; // 直前の時間を保存
unsigned long touchStartTime = 0; // タッチ開始時間を保存
const long interval = 10; // インターバル時間を設定（10ms）
const int threshold = 50; // 移動量の閾値を設定
const unsigned long rightClickThreshold = 100; // 右クリック判定の時間閾値（ms）

char receivedText[64]; // 受信データを格納するバッファ

void setup() {
  // 初期化
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("BLE Mouse");

  bleMouse.begin();
  while (bleMouse.isConnected() == false) {
    delay(100);
  }
}

void loop() {
  unsigned long currentMillis = millis(); // 現在の時間を取得

  if (Serial.available()) {
    // データを受信する
    int bytesRead = Serial.readBytesUntil('\n', receivedText, sizeof(receivedText) - 1);
    receivedText[bytesRead] = '\0'; // Null終端を追加して文字列を完成させる

    // 3つのintとして受け取る処理
    int values[3];
    int i = 0;
    char* p = strtok(receivedText, ", "); // mutableな文字列を使う

    while (p != NULL && i < 3) {
      values[i++] = atoi(p);
      p = strtok(NULL, ", ");
    }

    // 受け取った値を使って何かする場合の処理
    if (i == 3) {
      int value1 = values[0];
      int value2 = values[1];
      int value3 = values[2];
      Serial.printf("Received values: %d, %d, %d\n", value1, value2, value3);
    }
  }

  // タッチ座標処理
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    TouchPoint_t atTouchPoint;
    M5.update();
    if (M5.Touch.ispressed()) {
      // タッチしたX座標、Y座標を取得する
      atTouchPoint = M5.Touch.getPressPoint();
      int current_x = atTouchPoint.x;
      int current_y = atTouchPoint.y;

      // タッチ開始時間を記録
      if (touchStartTime == 0) {
        touchStartTime = millis();
      }

      // 以前の座標と比較して移動量を計算
      if (prev_x != -1 && prev_y != -1) {
        int delta_x = current_x - prev_x;
        int delta_y = current_y - prev_y;

        // 移動量が閾値を超えない場合のみマウス移動
        if (abs(delta_x) <= threshold && abs(delta_y) <= threshold) {
          bleMouse.move(delta_x, delta_y);
        }
      }

      // 現在の座標を前回の座標として保存
      prev_x = current_x;
      prev_y = current_y;
    } else {
      // タッチが解除された場合、前回の座標をリセット
      prev_x = -1;
      prev_y = -1;

      // タッチ開始時間が設定されている場合、右クリック判定を行う
      if (touchStartTime != 0 && (millis() - touchStartTime < rightClickThreshold)) {
        // 右クリックの操作
        bleMouse.press(MOUSE_LEFT);
        bleMouse.release(MOUSE_LEFT);
      }

      // タッチ開始時間をリセット
      touchStartTime = 0;
    }
  }
}
