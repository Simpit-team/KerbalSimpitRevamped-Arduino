#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>

struct altitudeMessage {
  float sealevel;
  float surface;
} __attribute__((packed));

struct apsidesMessage {
  float periapsis;
  float apoapsis;
} __attribute__((packed));

altitudeMessage parseAltitude(byte msg[]);
apsidesMessage parseApsides(byte msg[]);

#endif
