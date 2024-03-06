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

int lastSwitchState = LOW;
bool lastLEDState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);   // set the pushbutton pin to be an input
  pinMode(2, OUTPUT);  // set the yellow LED pin to be an output

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

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(topic);
  Serial.println();

  // subscribe to topics
  mqttClient.subscribe(topic);
  mqttClient.subscribe(topic2);
  mqttClient.subscribe(topic3);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

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

  int switchState = digitalRead(3);

  // Update LED state only if switch state changes or a new MQTT message is received
  if (switchState != lastSwitchState || lastLEDState != digitalRead(2)) {
    digitalWrite(2, switchState); // Update LED to switch state
    lastSwitchState = switchState; // Update last switch state
    lastLEDState = digitalRead(2); // Update last LED state
    Serial.println(switchState);   // Print switch state
  }
}

void onMqttMessage(int messageSize) {
  String message;
  while (mqttClient.available()) {
    message += (char)mqttClient.read();
  }

  Serial.print("Received a message from MQTT: ");
  Serial.println(message);

  // Interpret the message as an integer and update LED state accordingly
  int parsedValue = message.toInt();
  digitalWrite(2, parsedValue);   // Update LED to parsed value
  lastLEDState = parsedValue;      // Update last LED state
}
