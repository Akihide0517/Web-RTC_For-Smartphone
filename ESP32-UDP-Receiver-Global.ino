#include <M5Unified.h>
#include <WiFi.h>
#include <WiFiUdp.h>
void WiFiConnect();

const char* ssid = "ssid";
const char* password = "password";

WiFiUDP wifiUdp;
const int myPort = 1024; // 1024～60253を設定->本来はこの範囲でrandomが良い

void setup() {
  M5.begin();
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  WiFiConnect();
  wifiUdp.begin(myPort);
}

void loop() {
  if(wifiUdp.parsePacket()){
    char s;
    s = (char)wifiUdp.read();
    M5.Lcd.println(s);
  }
}

void WiFiConnect(){
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    M5.Lcd.print('.');
  }

  M5.Lcd.print("\r\nWiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());
  M5.Lcd.printf("myPort:%d", myPort);
}
