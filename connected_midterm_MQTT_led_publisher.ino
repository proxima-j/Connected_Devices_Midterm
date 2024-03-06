#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>

WiFiClient wifi;
MqttClient mqtt(wifi);

int lastSwitchState = LOW;

// WiFi network info: SSID and password
const char wifi_ssid[] = "sandbox370";
const char wifi_pass[] = "+s0a+s03!2gether?";

// MQTT broker info: URL and port (1883 default for MQTT)
const char broker[] = "theyonetwork.cloud.shiftr.io";
const int port = 1883;

// MQTT authentication credentials
const char mqtt_user[] = "theyonetwork";
const char mqtt_pass[] = "ConnDevSP24";

// Topic this device will publish messages to
const char pubTopic[] = "caaaaara";

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);   // Set the pushbutton pin to be an input
  pinMode(2, OUTPUT);  // Set the yellow LED pin to be an output

  Serial.print("Attempting to connect to WiFi");
  while (WiFi.begin(wifi_ssid, wifi_pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nWiFi connected!");

  // Set MQTT credentials
  mqtt.setUsernamePassword(mqtt_user, mqtt_pass);

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqtt.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqtt.connectError());
    while (true) {}; // Do nothing forever
  }

  Serial.println("Connected to the MQTT broker!");

  // Subscribe to topics if needed
  // mqtt.subscribe("topicName");

  Serial.println("Setup complete!");
}

void loop() {
  int switchState = digitalRead(3);

  if (switchState != lastSwitchState) {
    digitalWrite(2, HIGH);
    lastSwitchState = switchState;
    
    // Send MQTT message when switch state changes
    mqtt.beginMessage(pubTopic);
    mqtt.print(switchState);
    mqtt.endMessage();
    delay(10); // Add a small delay to allow the message to be sent properly
  } else {
    // If the switch is open or closed:
    digitalWrite(2, LOW);  // Turn off the yellow LED
  }
}
