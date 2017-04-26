#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>

struct altitudeStruct {
  float altitude;
  float surfaceAltitude;
};

altitudeStruct parseAltitude(byte msg[]);

#endif
