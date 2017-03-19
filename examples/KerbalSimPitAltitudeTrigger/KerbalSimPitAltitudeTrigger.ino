/* KerbalSimPitAltitudeTrigger

   Subscribes to the altitude channel.
   Turns on pin 13 LED when altitude > 5km.

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
  if (mySimPit.init()) {
    mySimPit.inboundHandler(packetHandler);
    mySimPit.registerChannel(ALTITUDE_PACKET);
  } else {
    // No connection. Abort.
    while (true) {
      digitalWrite(13, HIGH);
      delay(50);
      digitalWrite(13, LOW);
      delay(50);
    }
  }
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
