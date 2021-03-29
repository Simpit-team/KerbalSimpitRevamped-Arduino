/* KerbalSimpitThrottleDemo

   A demonstration of how to use KerbalSimPit to send throttle 
   or rotation command.

   Assume A0, A1, A2 are plugged on a potentiometer. Each potentiometer will be used
   to control throttle, pitch and roll of you rocket.
   See these links for basic descriptions of how to hook up a potentiometer:
     - https://www.arduino.cc/en/tutorial/potentiometer
     - https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-experiment-guide---v40/circuit-1b-potentiometer
*/

#include "KerbalSimpit.h"

// Set the pin numbers:
const int THROTTLE_PIN = A0; // the pin used for controlling throttle
const int PITCH_PIN = A1;    // the pin used for controlling pitch
const int ROLL_PIN = A2;     // the pin used for controlling roll

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set initial pin states  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Turn on the built-in to indicate the start of the handshake process
  digitalWrite(LED_BUILTIN, HIGH); 

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
  // Send at each loop a message to control the throttle and the pitch/roll axis.
  
  throttleMessage throttle_msg;
  // Read the value of the potentiometer
  int reading = analogRead(THROTTLE_PIN);
  // Convert it in KerbalSimpit Range
  throttle_msg.throttle = map(reading, 0, 1023, 0, INT16_MAX);
  // Send the message
  mySimpit.send(THROTTLE_MESSAGE, throttle_msg);

  rotationMessage rot_msg;
  // Read the values of the potentiometers
  int reading_pitch = analogRead(PITCH_PIN);
  int reading_roll = analogRead(ROLL_PIN);
  // Convert them in KerbalSimpit range
  int16_t pitch = map(reading_pitch, 0, 1023, INT16_MIN, INT16_MAX);
  int16_t roll = map(reading_roll, 0, 1023, INT16_MIN, INT16_MAX);
  // Put those values in the message
  rot_msg.setPitch(pitch);
  rot_msg.setRoll(roll);
  // Send the message
  mySimpit.send(ROTATION_MESSAGE, rot_msg);
}
