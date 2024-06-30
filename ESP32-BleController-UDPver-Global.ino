#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <BleConnectionStatus.h>
#include <BleMouse.h>

// 注意:"partition scheme"は"Minimal SPIFFS"にしてください

const char* ssid = "ssid";
const char* password = "pass";

WiFiUDP wifiUdp;
const int udpPort = 1024; // 1024～60253の範囲でポートを設定

BleMouse bleMouse;
int prev_x = -1;
int prev_y = -1;
unsigned long previousMillis = 0; // 移動直前の時間
const long interval = 10; // インターバル時間
const int threshold = 50; // 移動量の閾値

// UDPで受け取った数の保存先
int value1 = 0; // x座標
int value2 = 0; // y座標
int value3 = 0; // クリックの判定

bool UDPReceive = false; // 受信してるかどうか

char receivedText[64]; // バッファ

void setup() {
  // 初期化
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("BLE Mouse");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print('.');
  }

  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.printf("UDP Port: %d\n", udpPort);
  wifiUdp.begin(udpPort); // UDPのリスニング開始

  bleMouse.begin();
  while (bleMouse.isConnected() == false) {
    delay(100);
  }
}

void loop() {
  unsigned long currentMillis = millis(); // 状態遷移時間の保存

  int packetSize = wifiUdp.parsePacket(); // 受信パケットのサイズを取得
  if (packetSize) {
    int len = wifiUdp.read(receivedText, sizeof(receivedText) - 1); // データを受信
    if (len > 0) {
      receivedText[len] = '\0'; // 文字列の終端を追加
    }

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
      UDPReceive = true;
    }
  }

  // タッチ座標処理
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    M5.update();
    if (UDPReceive) {
      // X座標、Y座標を取得する
      int current_x = 0;
      int current_y = 0;

      // 以前の座標と比較して移動量を計算
      if (prev_x != -1 && prev_y != -1) {
        int delta_x = current_x - prev_x;
        int delta_y = current_y - prev_y;

        // 移動量が閾値を超えない場合のみマウス移動
        if (abs(delta_x) <= threshold && abs(delta_y) <= threshold) {
          bleMouse.move(delta_x, delta_y);
        }
      } else {
        // UDP通信で受信した場合は直接入れる
        int delta_x = value1;
        int delta_y = value2;

        bleMouse.move(delta_x, delta_y);
      }

      // 現在の座標を前回の座標として保存
      prev_x = current_x;
      prev_y = current_y;

      UDPReceive = false;
    } else {
      // タッチが解除された場合、前回の座標をリセット
      prev_x = -1;
      prev_y = -1;
    }

    // ボタンクリック処理(value3を見て決定)
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
