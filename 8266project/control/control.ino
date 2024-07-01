#include <ESP8266WiFi.h>

const char *ssid = "111"; 
const char *password = "13286208286"; 
const int control_port = 12345; // 网关设备的监听端口
IPAddress control_IP(192, 168, 137, 25);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 137, 1);

WiFiServer controlServer(control_port); // TCP服务器监听端口
WiFiClient controlClient;

void setup() {
  Serial.begin(115200);
  
  if (!WiFi.config(control_IP, gateway, subnet, dns)) {
      Serial.println("Failed to configure static IP");
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  controlServer.begin();
}

void loop() {
  controlClient = controlServer.available();
  if (controlClient) {
    String command = controlClient.readStringUntil('\n');
    command.trim(); // 确保命令前后没有空格和换行符
    Serial.print("Received command: ");
    Serial.println(command); // 调试信息

    if (command.equals("1")) {
      Serial.println("1");
    } else if (command.equals("0")) {
      Serial.println("0");
    } else {
      Serial.println("Invalid command received");
    }

    delay(100); // 确保STM32有时间处理和响应

    // 检查是否有来自STM32的响应
    if (Serial.available() > 0) {
      String response = Serial.readStringUntil('\n');
      controlClient.print(response);
      Serial.print("Sent response to gateway: ");
      Serial.println(response);
    } else {
      controlClient.print("No response");
      Serial.println("No response from STM32");
    }
  }
}
