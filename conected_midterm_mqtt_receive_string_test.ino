#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

char ssid[] = "sandbox370";        // your network SSID
char pass[] = "+s0a+s03!2gether?";    // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "theyonetwork.cloud.shiftr.io";
int port = 1883;
const char topic[] = "caaaaara";
const char topic2[] = "real_unique_topic_2";
const char topic3[] = "real_unique_topic_3";

const char mqtt_user[] = "theyonetwork";
const char mqtt_pass[] = "ConnDevSP24";

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);
  
  mqttClient.setUsernamePassword(mqtt_user, mqtt_pass);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  mqttClient.onMessage(onMqttMessageWrapper);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  mqttClient.subscribe(topic);
  mqttClient.subscribe(topic2);
  mqttClient.subscribe(topic3);

  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Topic: ");
  Serial.println(topic2);
  Serial.print("Topic: ");
  Serial.println(topic3);

  Serial.println();
}

void loop() {
  mqttClient.poll();
}

void onMqttMessageWrapper(int messageSize) {
  String message;
  while (mqttClient.available()) {
    message += (char)mqttClient.read();
  }
  onMqttMessage(message);
}

void onMqttMessage(String message) {
  Serial.println("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', contents: ");
  Serial.println(message);
  Serial.println();
}
