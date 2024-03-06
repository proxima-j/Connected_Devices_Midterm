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

  // subscribe to a topic
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

  if (switchState != lastSwitchState) {
    digitalWrite(2, HIGH);
    lastSwitchState = switchState;
    Serial.println(switchState);
  } else {
    // if the switch is open:
    digitalWrite(2, LOW);  // turn off the yellow LED
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
  if (parsedValue == 0) {
    digitalWrite(2, LOW);   // Turn off the LED
  } else {
    digitalWrite(2, HIGH);  // Turn on the LED
  }
}
