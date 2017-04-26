/* KerbalSimPitStageDemo
   This sketch demonstrates sending action group commands,
   by implementing a simple staging button.

   This is done by making some very minor modifications to
   the "Debounce" demo sketch included in the Arduino IDE.

   Hardware:
   * A momentary pushbutton connecting pin 1 to ground.
     (we're using the internal pull-up resistor on that pin)
   * (optional) an LED on pin 8.
*/
#include "KerbalSimPit.h"

// KerbalSimPit object
KerbalSimPit mySimPit(Serial);

// The pin the button is connected to
const int buttonPin = 2;

// The variable we store the most recent button reading in
int buttonState;
// This variable holds the last button state to compare against
int lastButtonState = HIGH;
// Whether or not we have told KSP to activate the next stage
bool stageSent = false;

// the last time the output pin was toggled
long lastDebounceTime = 0;
// the debounce time; increase if the output flickers
long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }

  // We're using an internal pull-up, so
  // we have to invert this logic.
  if (buttonState) {
    // Button is not pressed, so reset the
    // stageSent trigger.
    stageSent = false;
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    // Button is pressed. Send a stage event
    // if we haven't already.
    if (!stageSent) {
      mySimPit.activateAction(STAGE_ACTION);
      stageSent = true;
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  lastButtonState = reading;
}

    
