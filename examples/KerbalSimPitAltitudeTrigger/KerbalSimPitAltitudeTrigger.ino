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
    altitudeStruct myAltitude;
    myAltitude = parseAltitude(msg);
    if (myAltitude.altitude > 500) {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
    break;
  }
}
