/* KerbalSimPitStageDemo
   A simple staging button.
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
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  mySimPit.init();
}

void loop() {
  // Polling the button state
  int reading = digitalRead(buttonPin);
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

    