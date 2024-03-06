int lastSwitchState = LOW;


void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);   // set the pushbutton pin to be an input
  pinMode(2, OUTPUT);  // set the yellow LED pin to be an output
}

void loop() {
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
