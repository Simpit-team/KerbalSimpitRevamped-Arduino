/** @file PayloadStructs.cpp
    Functions to parse messages in to payload structs.
*/
#include "PayloadStructs.h"
#include "KerbalSimpitMessageTypes.h"

cagStatusMessage parseCAGStatusMessage(byte msg[]){
  cagStatusMessage* cagstatus;
  cagstatus = (cagStatusMessage*)msg;
  return *cagstatus;
}


SASInfoMessage parseSASInfoMessage(byte msg[]){
  SASInfoMessage* sasinfo;
  sasinfo = (SASInfoMessage*)msg;
  return *sasinfo;
}

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

orbitInfoMessage parseOrbitInfo(byte msg[]) {
  orbitInfoMessage* orbitInfo;
  orbitInfo = (orbitInfoMessage*)msg;
  return *orbitInfo;
}

resourceMessage parseResource(byte msg[]) {
  resourceMessage* resource;
  resource = (resourceMessage*)msg;
  return *resource;
}

TACLSResourceMessage parseTACLSResource(byte msg[]) {
  TACLSResourceMessage* resource;
  resource = (TACLSResourceMessage*)msg;
  return *resource;
}

TACLSWasteMessage parseTACLSWaste(byte msg[]) {
  TACLSWasteMessage* resource;
  resource = (TACLSWasteMessage*)msg;
  return *resource;
}

CustomResourceMessage parseCustomResource(byte msg[]) {
  CustomResourceMessage* resource;
  resource = (CustomResourceMessage*)msg;
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

maneuverMessage parseManeuver(byte msg[]) {
  maneuverMessage* maneuver;
  maneuver = (maneuverMessage*)msg;
  return *maneuver;
}

vesselPointingMessage parseRotation(byte msg[]) {
  vesselPointingMessage* rot;
  rot = (vesselPointingMessage*)msg;
  return *rot;
}

deltaVMessage parseDeltaV(byte msg[]) {
  deltaVMessage* deltaV;
  deltaV = (deltaVMessage*)msg;
  return *deltaV;
}

deltaVEnvMessage parseDeltaVEnv(byte msg[]) {
  deltaVEnvMessage* deltaVEnv;
  deltaVEnv = (deltaVEnvMessage*)msg;
  return *deltaVEnv;
}

burnTimeMessage parseBurnTime(byte msg[]) {
  burnTimeMessage* burnTime;
  burnTime = (burnTimeMessage*)msg;
  return *burnTime;
}

tempLimitMessage parseTempLimitMessage(byte msg[]){
  tempLimitMessage* tempLimit;
  tempLimit = (tempLimitMessage*)msg;
  return *tempLimit;
}

flightStatusMessage parseFlightStatusMessage(byte msg[]){
  flightStatusMessage* flightStatus;
  flightStatus = (flightStatusMessage*)msg;
  return *flightStatus;
}

// Helper functions for using bitmask

rotationMessage::rotationMessage(){
  this->mask = 0;
}

void rotationMessage::setPitch(int16_t pitch){
  this->pitch = pitch;
  mask += PITCH_ROT;
}

void rotationMessage::setRoll(int16_t roll){
  this->roll = roll;
  mask += ROLL_ROT;
}

void rotationMessage::setYaw(int16_t yaw){
  this->yaw = yaw;
  mask += YAW_ROT;
}

void rotationMessage::setPitchRollYaw(int16_t pitch, int16_t roll, int16_t yaw){
  this->pitch = pitch;
  this->roll = roll;
  this->yaw = yaw;
  mask = PITCH_ROT + ROLL_ROT + YAW_ROT;
}

translationMessage::translationMessage(){
  this->mask = 0;
}

void translationMessage::setX(int16_t x){
  this->X = x;
  mask += X_TRANS;
}

void translationMessage::setY(int16_t y){
  this->Y = y;
  mask += Y_TRANS;
}

void translationMessage::setZ(int16_t z){
  this->Z = z;
  mask += Z_TRANS;
}

void translationMessage::setXYZ(int16_t x, int16_t y, int16_t z){
  this->X = x;
  this->Y = y;
  this->Z = z;
  mask = X_TRANS + Y_TRANS + Z_TRANS;
}

customAxisMessage::customAxisMessage(){
  this->mask = 0;
}

void customAxisMessage::setCustom1(int16_t val){
  this->custom1 = val;
  mask |= 1;
}

void customAxisMessage::setCustom2(int16_t val){
  this->custom2 = val;
  mask |= 2;
}

void customAxisMessage::setCustom3(int16_t val){
  this->custom3 = val;
  mask |= 4;
}

void customAxisMessage::setCustom4(int16_t val){
  this->custom4 = val;
  mask |= 8;
}

void customAxisMessage::setAllCustom(int16_t custom1, int16_t custom2, int16_t custom3, int16_t custom4){
  this->custom1 = custom1;
  this->custom2 = custom2;
  this->custom3 = custom3;
  this->custom4 = custom4;
  mask = 1 + 2 + 4 + 8;
}

wheelMessage::wheelMessage(){
  this->mask = 0;
}

void wheelMessage::setSteer(int16_t steer){
  this->steer = steer;
  mask += STEER_WHEEL;
}

void wheelMessage::setThrottle(int16_t throttle){
  this->throttle = throttle;
  mask += THROTTLE_WHEEL;
}

void wheelMessage::setSteerThrottle(int16_t steer, int16_t throttle){
  this->steer = steer;
  this->throttle = throttle;
  mask = STEER_WHEEL + THROTTLE_WHEEL;
}

cameraRotationMessage::cameraRotationMessage(){
  this->mask = 0;
}

void cameraRotationMessage::setPitch(int16_t pitch){
  this->cameraPitch = pitch;
  mask += PITCH_ROT;
}

void cameraRotationMessage::setRoll(int16_t roll){
  this->cameraRoll = roll;
  mask += ROLL_ROT;
}

void cameraRotationMessage::setYaw(int16_t yaw){
  this->cameraYaw = yaw;
  mask += YAW_ROT;
}

void cameraRotationMessage::setZoom(int16_t zoom){
  this->cameraZoom = zoom;
  mask += ZOOM_ROT;
}

void cameraRotationMessage::setPitchRollYawZoom(int16_t pitch, int16_t roll, int16_t yaw, int16_t zoom){
  this->cameraPitch = pitch;
  this->cameraRoll = roll;
  this->cameraYaw = yaw;
  this->cameraZoom = zoom;
  mask = PITCH_ROT + ROLL_ROT + YAW_ROT + ZOOM_ROT;
}

keyboardEmulatorMessage::keyboardEmulatorMessage(int16_t keyCode) : keyboardEmulatorMessage(keyCode, 0) {}

keyboardEmulatorMessage::keyboardEmulatorMessage(int16_t keyCode, byte modifier){
  this->keyCode = keyCode;
  this->modifier = modifier;
}
