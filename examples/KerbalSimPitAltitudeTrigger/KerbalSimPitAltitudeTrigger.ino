/* KerbalSimPitAltitudeTrigger
   A demonstration of subscribing to telemetry data from the game.
   Subscribes to the altitude channel, and turns the pin 13 LED
   on when the sea level altitude > 500m.

   Peter Hardy <peter@hardy.dropbear.id.au>
*/
#include "KerbalSimPit.h"

// KerbalSimPit object
KerbalSimPit mySimPit(Serial);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!mySimPit.init()) {
    delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
  mySimPit.inboundHandler(packetHandler);
  mySimPit.registerChannel(ALTITUDE_PACKET);
}

void loop() {
  mySimPit.update();
}

void packetHandler(byte packetType, byte msg[], byte msgSize) {
  switch(packetType) {
  case ALTITUDE_PACKET:
    if (msgSize == sizeof(altitudeMessage)) {
      altitudeMessage myAltitude;
      myAltitude = parseAltitude(msg);
      if (myAltitude.altitude > 500) {
        digitalWrite(LED_BUILTIN, HIGH);
      } else {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
    break;
  }
}
