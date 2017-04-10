/* KerbalSimPitHelloWorld
   A very barebones Hello World sketch, that doesn't require
   any extra hardware. It periodically sends EchoRequest packets
   to the game, and uses the EchoReply packets to switch the
   on-board LED on and off. Note that the game only activates the
   echo handler during the flight scene.
   Great for confirming basic connectivity.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimPit.h"

// The built-in LED output pin
const int ledPin = 13;

// KerbalSimPit object
KerbalSimPit mySimPit(115200);

// tracking desired LED state
bool state = false;

// When an echo packet was last sent
unsigned long lastSent = 0;
// How often to send echo packets (in ms)
unsigned int sendInterval = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(ledPin, LOW);
  mySimPit.inboundHandler(packetHandler);
}

void loop() {
  unsigned long now = millis();
  if (now - lastSent >= sendInterval) {
    if (state) {
      mySimPit.send(ECHO_REQ_PACKET, "low", 4);
    } else {
      mySimPit.send(ECHO_REQ_PACKET, "high", 5);
    }
    lastSent = now;
    state = !state;
  }
  mySimPit.update();
}

void packetHandler(byte packetType, byte *msg, byte msgSize) {
  if (packetType == ECHO_RESP_PACKET) {
    if (strcmp(msg, "low")) {
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, HIGH);
    }
  }
}
