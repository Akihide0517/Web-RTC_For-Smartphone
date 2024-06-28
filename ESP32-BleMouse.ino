#include <BleConnectionStatus.h>
#include <BleMouse.h>
#include <M5Core2.h>

BleMouse bleMouse;
int prev_x = -1;
int prev_y = -1;
unsigned long previousMillis = 0; // 移動直前の時間
const long interval = 10; // インターバル時間
const int threshold = 50; // 移動量の閾値

//シリアルで受け取った数の保存先
int value1 = 0;// x座標
int value2 = 0;// y座標
int value3 = 0;

bool SerialMode = false;//シリアルで受信するかどうか
bool SerialRecive = false;//受信してるかどうか

char receivedText[64]; // バッファ

void setup() {
  // 初期化
  Serial.begin(115200);
  M5.begin();
  M5.Lcd.setRotation(3);
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
  unsigned long currentMillis = millis(); // 状態遷移時間の保存

  if (Serial.available()) {
    // データを受信する
    int bytesRead = Serial.readBytesUntil('\n', receivedText, sizeof(receivedText) - 1);
    receivedText[bytesRead] = '\0';

    // 3つのintとして受け取る処理(12, 12, 12の場合)
    int values[3];
    int i = 0;
    char* p = strtok(receivedText, ", ");

    while (p != NULL && i < 3) {
      values[i++] = atoi(p);
      p = strtok(NULL, ", ");
    }

    // 受け取った値
    if (i == 3) {
      value1 = values[0];
      value2 = values[1];
      value3 = values[2];
      Serial.printf("Received values: %d, %d, %d\n", value1, value2, value3);
      SerialMode = false;
      SerialRecive = true;
    }
  }

  // タッチ座標処理
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    TouchPoint_t atTouchPoint;
    M5.update();
    if (M5.Touch.ispressed() || SerialRecive == true) {
      // タッチしたX座標、Y座標を取得する
      atTouchPoint = M5.Touch.getPressPoint();
      int current_x = atTouchPoint.x;
      int current_y = atTouchPoint.y;

      // 以前の座標と比較して移動量を計算
      if (prev_x != -1 && prev_y != -1 && SerialMode == false) {
        int delta_x = current_x - prev_x;
        int delta_y = current_y - prev_y;

        // 移動量が閾値を超えない場合のみマウス移動
        if (abs(delta_x) <= threshold && abs(delta_y) <= threshold) {
          bleMouse.move(delta_x, delta_y);
        }

      //シリアル通信の場合は直接入れる
      }else {
        int delta_x = value1;
        int delta_y = value2;

        bleMouse.move(delta_x, delta_y);
      }

      // 現在の座標を前回の座標として保存
      prev_x = current_x;
      prev_y = current_y;

      SerialRecive = false;
    } else {
      // タッチが解除された場合、前回の座標をリセット
      prev_x = -1;
      prev_y = -1;
    }

    // ボタンクリック処理
    if (M5.BtnA.isPressed() || value3 == 1) {
      if (M5.BtnB.isPressed()) {
        bleMouse.press(MOUSE_LEFT | MOUSE_RIGHT);
      } else {
        bleMouse.release(MOUSE_RIGHT);
        bleMouse.press(MOUSE_LEFT);
      }
    } else {
      if (M5.BtnB.isPressed() || value3 == 2) {
        bleMouse.release(MOUSE_LEFT);
        bleMouse.press(MOUSE_RIGHT);
      } else {
        bleMouse.release(MOUSE_LEFT | MOUSE_RIGHT);
      }
    }
  }
}
