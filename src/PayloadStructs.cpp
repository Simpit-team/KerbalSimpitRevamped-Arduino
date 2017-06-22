/** @file PayloadStructs.cpp
    Functions to parse messages in to payload structs.
*/
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

apsidesTimeMessage parseApsidesTime(byte msg[]) {
  apsidesTimeMessage* apsidesTime;
  apsidesTime = (apsidesTimeMessage*)msg;
  return *apsidesTime;
}

resourceMessage parseResource(byte msg[]) {
  resourceMessage* resource;
  resource = (resourceMessage*)msg;
  return *resource;
}

velocityMessage parseVelocity(byte msg[]) {
  velocityMessage* velocity;
  velocity = (velocityMessage*)msg;
  return *velocity;
}

targetMessage parseTarget(byte msg[]) {
  targetMessage* target;
  target = (targetMessage*)msg;
  return *target;
}

airspeedMessage parseAirspeed(byte msg[]) {
  airspeedMessage* airspeed;
  airspeed = (airspeedMessage*)msg;
  return *airspeed;
}
