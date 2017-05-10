/* KerbalSimPitStageDemo

   This sketch enhances the "Debounce" Arduino demo sketch
   to implement a simple staging button.

   See https://www.arduino.cc/en/tutorial/debounce for
   hardware requirements and how to get debounce up and
   running, then return and give this code a try with KSP.

   Note: Unlike the debounce example, this sketch will
   light the LED while the button is pressed.

*/
#include "KerbalSimPit.h"

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured
// in miliseconds, will quickly become a bigger number than can be stored
// in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin
                                     // was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase
                                     // if the output flickers

// Create a KerbalSimPit object.
KerbalSimPit mySimPit(Serial);

void setup() {
  // Start the serial connection.
  Serial.begin(115200);

  pinMode(buttonPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set initial LED state.
  digitalWrite(ledPin, HIGH);
  // Attempt to establish a connection with the game plugin.
  // We continually retry this until we get a successful handshake.
  while (!mySimPit.init()) {
    delay(100);
  }
  // Turn the LED off to indicate successful handshake.
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // Read the state of the switch into a local variable.
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only send a stage command if the new button state is HIGH
      if (buttonState == HIGH) {
        mySimPit.activateAction(STAGE_ACTION);
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, buttonState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}
