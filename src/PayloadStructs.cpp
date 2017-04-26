#include "PayloadStructs.h"

altitudeMessage parseAltitude(byte msg[]) {
  altitudeMessage* alt;
  alt = (altitudeMessage*)msg;
  return *alt;
}
