#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char *ssid = "111"; // TODO: 更改为你的WiFi网络SSID
const char *password = "13286208286"; // TODO: 更改为你的WiFi网络密码
const char* gateway_ip = "192.168.137.93"; // TODO: 更改为ESP8266网关的IP地址
const int gateway_port = 12345; // TODO: 更改为ESP8266网关接收数据的端口

WiFiClient client;

void setup() {
  Serial.begin(115200); // 使用硬件串口

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // 打印ESP8266的IP地址
}

void loop() {
  if (Serial.available()) {
    String jsonData = Serial.readString(); // 从硬件串口读取数据

    if (client.connect(gateway_ip, gateway_port)) {
      client.print(jsonData);
      client.stop();
    }
  }
}
