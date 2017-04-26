#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>

struct altitudeMessage {
  float altitude;
  float surfaceAltitude;
} __attribute__((packed));

altitudeMessage parseAltitude(byte msg[]);

#endif
