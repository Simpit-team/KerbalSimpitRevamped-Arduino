/* KerbalSimPitStageDemo
   A simple staging button. This is intended to be a small
   addition to the "Debounce" demo sketch included with the
   Arduino IDE.

   Hardware:
   * A momentary pushbutton connecting pin 1 to ground.
     (we're using the internal pull-up resistor on that pin)
   * (optional) an LED on pin 8.
*/
#include "KerbalSimPit.h"

// KerbalSimPit object
KerbalSimPit mySimPit(115200);

// The pin the button is connected to
const int buttonPin = 1;
// The pin connected to an optional status LED
const int ledPin = 8;

// The current reading from the input pin
int buttonState;
// The previous reading from the input pin
int lastButtonState = LOW;

// the last time the output pin was toggled
long lastDebounceTime = 0;
// the debounce time; increase if the output flickers
long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(ledPin, LOW);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
    // If the current button state is low, and the
    // previous button state is high, then the button
    // has just been pressed. So send a stage event.
    if (!buttonState && lastButtonState) {
      mySimPit.stageEvent();
    }
  }

  digitalWrite(ledPin, buttonState);
  lastButtonState = buttonState;
}

    