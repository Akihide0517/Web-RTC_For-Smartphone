#include <BleConnectionStatus.h>
#include <BleMouse.h>
#include <M5Core2.h>

BleMouse bleMouse;
int prev_x = -1;
int prev_y = -1;
unsigned long previousMillis = 0; // 直前の時間を保存
const long interval = 10; // インターバル時間を設定（10ms）
const int threshold = 50; // 移動量の閾値を設定

void setup() {
  // 初期化
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
  unsigned long currentMillis = millis(); // 現在の時間を取得

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
    }

    // ボタンクリック処理
    if (M5.BtnA.isPressed()) {
      if (M5.BtnB.isPressed()) {
        bleMouse.press(MOUSE_LEFT | MOUSE_RIGHT);
      } else {
        bleMouse.release(MOUSE_RIGHT);
        bleMouse.press(MOUSE_LEFT);
      }
    } else {
      if (M5.BtnB.isPressed()) {
        bleMouse.release(MOUSE_LEFT);
        bleMouse.press(MOUSE_RIGHT);
      } else {
        bleMouse.release(MOUSE_LEFT | MOUSE_RIGHT);
      }
    }
  }
}
