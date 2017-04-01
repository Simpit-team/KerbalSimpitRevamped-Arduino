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

// 
KerbalSimPit mySimPit(115200);

const int buttonPin = 1;
const int ledPin = 8;

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // 
  while (!mySimPit.init()) {
    delay(100);
  }
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

    