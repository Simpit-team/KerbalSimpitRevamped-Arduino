/* KerbalSimPitAltitudeTrigger
   A demonstration of subscribing to telemetry data from the game.
   Subscribes to the altitude channel, and turns the pin 13 LED
   on when the sea level altitude > 5km.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimPit.h"

// KerbalSimPit object
KerbalSimPit mySimPit(Serial);

const int ledPin = 13;

// This struct matches the format
// altitude data is sent from the game.
struct altitudeStruct {
  float alt;
  float surfalt;
};
altitudeStruct myAltitude;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(ledPin, LOW);
  mySimPit.inboundHandler(packetHandler);
  mySimPit.registerChannel(ALTITUDE_PACKET);
}

void loop() {
  mySimPit.update();
}

void packetHandler(byte packetType, byte *msg, byte msgSize) {
  switch(packetType) {
  case ALTITUDE_PACKET:
    if (msgSize == sizeof(myAltitude)) {
      altitudeHandler(msg);
    }
    break;
  }
}

void altitudeHandler(byte *msg) {
  memcpy(&myAltitude, msg, sizeof(myAltitude));
  if (myAltitude.alt > 500) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
