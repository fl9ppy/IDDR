// ----- Button Setup -----
const int buttonPin = 2;     // Button connected to D2
int lastState = HIGH;        // Because INPUT_PULLUP starts HIGH
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 30;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Debounce
  if (reading != lastState) {
    lastDebounce = millis();
  }

  if ((millis() - lastDebounce) > debounceDelay) {
    // Send only when state stabilized
    if (reading != lastState) {
      lastState = reading;
      Serial.println(lastState);   // Send: 0 = pressed, 1 = released
    }
  }
}
