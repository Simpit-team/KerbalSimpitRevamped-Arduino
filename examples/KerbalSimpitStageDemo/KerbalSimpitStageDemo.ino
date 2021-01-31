/* KerbalSimpitStageDemo

   This sketch enhances the "Debounce" Arduino demo sketch
   to implement a simple staging button.

   See https://www.arduino.cc/en/tutorial/debounce for
   hardware requirements and how to get debounce up and
   running, then return and give this code a try with KSP.

   Note: Unlike the debounce example, this sketch will
   light the LED while the button is pressed.

*/
#include "KerbalSimpit.h"

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

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states, and turn on the LED
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(ledPin, HIGH);

  // This loop continually attempts to handshake with the plugin.
  // It will keep retrying until it gets a successful handshake.
  while (!mySimpit.init()) {
    delay(100);
  }
  // Turn off the built-in LED to indicate handshaking is complete.
  digitalWrite(LED_BUILTIN, LOW);
  // Display a message in KSP to indicate handshaking is complete.
  mySimpit.printToKSP("Connected", PRINT_TO_SCREEN);
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

      // If the new button state is HIGH, that means the button
      // has just been pressed.
      if (buttonState == HIGH) {
        // Send a message to the plugin activating the Stage
        // action group. The plugin will then activate the
        // next stage.
        mySimpit.activateAction(STAGE_ACTION);
      }
    }
  }

  // Set the LED to match the state of the button.
  digitalWrite(ledPin, buttonState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}
