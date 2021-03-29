/* KerbalSimpitHelloWorld
   A very barebones Hello World sketch, that doesn't require
   any extra hardware. It periodically sends EchoRequest packets
   to the game, and uses the EchoReply packets to switch the
   on-board LED on and off. Note that the game only activates the
   echo handler during the flight scene.
   Great for confirming basic connectivity.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimpit.h"

// Declare a KerbalSimpit object that will
// communicate using the "Serial" device.
KerbalSimpit mySimpit(Serial);

// This boolean tracks the desired LED state.
bool state = false;

// A timestamp of the last time we sent an echo packet.
unsigned long lastSent = 0;
// How often to send echo packets (in ms)
unsigned int sendInterval = 1000;

void setup() {
  // Open the serial connection.
  Serial.begin(115200);

  // Set up the built in LED, and turn it on.
  pinMode(LED_BUILTIN, OUTPUT);
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
  // Sets our callback function. The KerbalSimpit library will
  // call this function every time a packet is received.
  mySimpit.inboundHandler(messageHandler);
}

void loop() {
  unsigned long now = millis();
  // If we've waited long enough since the last message, send a new one.
  if (now - lastSent >= sendInterval) {
    // If the last message was "high", send "low"
    // and vice-versa.
    if (state) {
      mySimpit.send(ECHO_REQ_MESSAGE, "low", 4);
    } else {
      mySimpit.send(ECHO_REQ_MESSAGE, "high", 5);
    }
    // Update when we last sent a message.
    lastSent = now;
    // Update the state variable to match the message we just sent.
    state = !state;
  }
  // Call the library update() function to check for new messages.
  mySimpit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  // We are only interested in echo response packets.
  if (messageType == ECHO_RESP_MESSAGE) {
    // The message payload will be either "low" or "high".
    // We use the strcmp function to check what the string payload
    // is, and set the LED status accordingly.
    if (strcmp(msg, "low")) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}
