#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>

struct altitudeStruct {
  float altitude;
  float surfaceAltitude;
} __attribute__((packed));

altitudeStruct parseAltitude(byte msg[]);

#endif
