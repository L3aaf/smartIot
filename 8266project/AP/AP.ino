#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char *ssid = "111";
const char *password = "13286208286";
const char *mqtt_server = "dqwork.zitiu.top";
const int mqtt_port = 1883;
IPAddress local_IP(192, 168, 137, 93);
IPAddress contol_IP(192, 168, 137, 25);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 137, 1);

WiFiServer server(12345);  // TCP服务器监听端口
WiFiClient espClient;
WiFiClient controlClient;
PubSubClient mqttClient(espClient);

void setup_wifi() {
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
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
}

void controlLED(String ledState) {
  if (controlClient.connect(contol_IP, 12345)) {  // 确保是控制节点的IP地址和端口
    controlClient.print(ledState);
    Serial.print("Sent control command to control node: ");
    Serial.println(ledState);
  } else {
    Serial.println("Failed to connect to control node");
  }
}


/*
// 处理从MQTT服务器接收到的消息
// 解析JSON数据，根据内容控制LED或其他设备
*/
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println(message);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  int ledState = doc["LED_SW"];
  if (ledState == 1) {
    controlLED("1\n");
  } else if (ledState == 0) {
    controlLED("0\n");
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Gateway")) {
      Serial.println("connected");
      mqttClient.subscribe("ddq_sub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  server.begin();

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(callback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  WiFiClient sensorClient = server.available();
  if (sensorClient) {
    String jsonData = sensorClient.readStringUntil('\n');
    Serial.print("Received data from sensor node: ");
    Serial.println(jsonData);
    mqttClient.publish("ddq_pub", jsonData.c_str());
  }
}