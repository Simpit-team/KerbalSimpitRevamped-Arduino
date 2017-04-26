#include "PayloadStructs.h"

altitudeStruct parseAltitude(byte *msg) {
  altitudeStruct* alt;
  alt = (altitudeStruct *)msg;
  return *alt;
}
