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

KerbalSimPit mySimPit(115200);

const int buttonPin = 1;
const int ledPin = 8;

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

long lastDebounceTime = 0;
long debounceDelay = 50;
long lastSendTime = 0;
long sendDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  mySimPit.init();
}

void loop() {
  // Polling the button state
  byte reading;
  if (digitalRead(buttonPin)) {
    reading = 1;
  } else {
    reading = 0;
  }

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    buttonState = reading;
  }

  // Only send a packet every 50ms
  if ((millis() - lastSendTime) > sendDelay) {
    mySimPit.send(STAGE_PACKET, buttonState, sizeof(buttonState));
    lastSendTime = millis();
  }
  digitalWrite(ledPin, buttonState);
  lastButtonState = reading;
}

    