#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiUdp.h>
void WiFiConnect();

const char* ssid = "ssid";
const char* password = "password";

WiFiUDP wifiUdp;
const int myPort = 1024; // 1024～60253を設定

void setup() {
  M5.begin();
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  WiFiConnect();
  wifiUdp.begin(myPort);
}

void loop() {
  int packetSize = wifiUdp.parsePacket();
  if (packetSize) {
    char packetBuffer[255]; // 受信するデータ用のバッファ
    int len = wifiUdp.read(packetBuffer, 255); // データをバッファに読み取る
    if (len > 0) {
      packetBuffer[len] = 0; // 文字列の終端を追加
    }

    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Received UDP data:\n");
    M5.Lcd.println(packetBuffer); // 受信したデータを表示
  }
}

void WiFiConnect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print('.');
  }

  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.printf("myPort:%d", myPort);
}
