/* KerbalSimPitAltitudeTrigger
   A demonstration of subscribing to telemetry data from the game.
   Subscribes to the altitude channel, and turns the pin 13 LED
   on when the sea level altitude > 5km.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimPit.h"

KerbalSimPit mySimPit(115200);

struct altitudeStruct {
  float alt;
  float surfalt;
};

altitudeStruct myAltitude;

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  while (!mySimPit.init()) {
    delay(100);
  }
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
  if (myAltitude.alt > 5000) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
