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

// KerbalSimPit object
KerbalSimPit mySimPit(Serial);

// tracking desired LED state
bool state = false;

// When an echo packet was last sent
unsigned long lastSent = 0;
// How often to send echo packets (in ms)
unsigned int sendInterval = 1000;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
  mySimPit.inboundHandler(messageHandler);
}

void loop() {
  unsigned long now = millis();
  if (now - lastSent >= sendInterval) {
    if (state) {
      mySimPit.send(ECHO_REQ_MESSAGE, "low", 4);
    } else {
      mySimPit.send(ECHO_REQ_MESSAGE, "high", 5);
    }
    lastSent = now;
    state = !state;
  }
  mySimPit.update();
}

void messageHandler(byte messageType, byte msg[], byte msgSize) {
  if (messageType == ECHO_RESP_MESSAGE) {
    if (strcmp(msg, "low")) {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}
