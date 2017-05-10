#include "PayloadStructs.h"

altitudeMessage parseAltitude(byte msg[]) {
  altitudeMessage* alt;
  alt = (altitudeMessage*)msg;
  return *alt;
}

apsidesMessage parseApsides(byte msg[]) {
  apsidesMessage* apsides;
  apsides = (apsidesMessage*)msg;
  return *apsides;
}

resourceMessage parseResource(byte msg[]) {
  resourceMessage* resource;
  resource = (resourceMessage*)msg;
  return *resource;
}
