/** @file PayloadStructs.h
    Structs for compound message types.
*/
#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>
#include "KerbalSimpitMessageTypes.h"


/** An Altitude message. */
struct cagStatusMessage {
  byte status[32]; /**< List of all the action status organised by bytes. Read them with the is_action_activated method.*/

  bool is_action_activated(byte i){
    return bitRead(status[i/8], i%8);
  }
} __attribute__((packed));

/** An SAS info message to reprsent the current SAS state. */
struct SASInfoMessage {
  byte currentSASMode; /**< Current SAS mode. SAS modes are listed in the AutopilotMode enum. 255 is used to indicate a disabled SAS. */
  int16_t SASModeAvailability; /**< bitmask for the availability of each SAS mode. If a mode is not available, a SAS_MODE_MESSAGE setting this mode will be ignored. */
} __attribute__((packed));

/** An Altitude message. */
struct altitudeMessage {
  float sealevel; /**< Altitude above sea level. */
  float surface; /**< Surface altitude at current position. */
} __attribute__((packed));

/** An Apsides message. */
struct apsidesMessage {
  float periapsis; /**< Current vessel's orbital periapsis. */
  float apoapsis; /**< Current vessel's orbital apoapsis. */
} __attribute__((packed));

/** An Apsides Time message. */
struct apsidesTimeMessage {
  int32_t periapsis; /** Time until the current vessel's orbital periapsis, in seconds. */
  int32_t apoapsis; /** Time until the current vessel's orbital apoapsis, in seconds. */
} __attribute__((packed));

/** An message containing orbital information. */
struct orbitInfoMessage
{
  float eccentricity; /**< Current vessel's orbital eccentricity. */
  float semiMajorAxis; /**< Current vessel's orbital semi major axis. */
  float inclination; /**< Current vessel's orbital inclination. */
  float longAscendingNode; /**< Current vessel's orbital longitude of ascending node. */
  float argPeriapsis; /**< Current vessel's orbital argument of periapsis. */
  float trueAnomaly; /**< Current vessel's orbital true anomaly. */
  float meanAnomaly; /**< Current vessel's orbital mean anomaly. */
  float period; /**< Current vessel's orbital period. */
} __attribute__((packed));

/** An message containing information about the current flight. */
struct flightStatusMessage
{
  byte flightStatusFlags; /**< Different booleans as defined by FligthStatusFlags. You can access them with the helper funtions.*/
  byte vesselSituation; /**< Current situation of the vessel, as defined by the Vessel.Situations enum in the KSP API (1 for Landed, 8 for flying, etc.).*/
  byte currentTWIndex; /**< Current TW index */
  byte crewCapacity; /**< Current vessel crew total capacity */
  byte crewCount; /**< Current vessel crew count */
  byte commNetSignalStrenghPercentage; /**< Current vessel commNet signal strengh (in percentage). 0 when CommNet is not used */
  byte currentStage; /**< Current stage of the vessel (also the total number of stage remaining). Decreased by one at each staging. Caped at 255 by Simpit.*/
  byte vesselType; /**< Current type of the vessel (e.g. 0 for debris, 5 for rover, 3 for probe, 7 for ship, etc.) as indicated in https://www.kerbalspaceprogram.com/api/_vessel_8cs.html#afa39c7ec7cc0926b332fcd2d77425edb*/

  inline bool isInFlight(){ return this->flightStatusFlags & FLIGHT_IN_FLIGHT; }
  inline bool isInEVA(){ return this->flightStatusFlags & FLIGHT_IS_EVA; }
  inline bool isRecoverable(){ return this->flightStatusFlags & FLIGHT_IS_RECOVERABLE; }
  inline bool isInAtmoTW(){ return this->flightStatusFlags & FLIGHT_IS_ATMO_TW; }
  /** Returns the current control level. 0 for no control, 1 for partially unmanned, 2 for partially manned, 3 for full control. */
  inline byte getControlLevel() { return (this->flightStatusFlags >> 4) & 3; }
  inline bool hasTarget(){ return this->flightStatusFlags & FLIGHT_HAS_TARGET; }
} __attribute__((packed));

/** An message containing information about the current atmospheric conditions. */
struct atmoConditionsMessage
{
  byte atmoCharacteristics; /**< Different booleans as defined by AtmoConditionsFlags. You can access them with the helper funtions.*/
  float airDensity;   /**< Current density in the current atmosphere where the current vessel is. Only defined if in atmosphere.  */
  float temperature;  /**< Current temperature in the current atmosphere where the current vessel is. Only defined if in atmosphere.  */
  float pressure;     /**< Current pressure in the current atmosphere where the current vessel is. Only defined if in atmosphere.  */

  inline bool hasAtmosphere(){ return this->atmoCharacteristics & HAS_ATMOSPHERE; }
  inline bool hasOxygen(){ return this->atmoCharacteristics & HAS_OXYGEN; }
  inline bool isVesselInAtmosphere(){ return this->atmoCharacteristics & IS_IN_ATMOSPHERE; }
} __attribute__((packed));

/** A Resource message.
    All resource messages use this struct for sending data. */
struct resourceMessage {
  float total; /**< Maximum capacity of the resource. */
  float available; /**< Current resource level. */
} __attribute__((packed));

/** A Resource message for TACLS ressources. */
struct TACLSResourceMessage {
  float currentFood; /**< Current resource level for food. */
  float maxFood; /**< Maximum capacity of food. */
  float currentWater; /**< Current resource level for water. */
  float maxWater; /**< Maximum capacity of water. */
  float currentOxygen; /**< Current resource level for oxygen. */
  float maxOxygen; /**< Maximum capacity of oxygen. */
} __attribute__((packed));

/** A Resource message for TACLS ressources. */
struct TACLSWasteMessage {
  float currentWaste; /**< Current resource level for waste. */
  float maxWaste; /**< Maximum capacity of waste. */
  float currentLiquidWaste; /**< Current resource level for liquid waste. */
  float maxLiquidWaste; /**< Maximum capacity of liquid waste. */
  float currentCO2; /**< Current resource level for CO2. */
  float maxCO2; /**< Maximum capacity of CO2. */
} __attribute__((packed));

/** A Resource message for custom ressources. The resources must be set in the 
 *  configuration file.
 */
struct CustomResourceMessage {
  float currentResource1; /**< Current resource level resource 1. */
  float maxResource1; /**< Maximum capacity of resource 1. */
  float currentResource2; /**< Current resource level resource 2. */
  float maxResource2; /**< Maximum capacity of resource 2. */
  float currentResource3; /**< Current resource level resource 3. */
  float maxResource3; /**< Maximum capacity of resource 3. */
  float currentResource4; /**< Current resource level resource 4. */
  float maxResource4; /**< Maximum capacity of resource 4. */
} __attribute__((packed));

/** A Velocity message. */
struct velocityMessage {
  float orbital; /**< Orbital velocity. */
  float surface; /**< Surface velocity. */
  float vertical; /**< Vertical velocity. */
} __attribute__((packed));

/** A Target information message. */
struct targetMessage {
  float distance; /**< Distance to target. */
  float velocity; /**< Velocity relative to target. */
  float heading; /**< Heading of the target. */
  float pitch; /**< Pitch of the target. */
  float velocityHeading; /**< Heading of the velocity to the target. */
  float velocityPitch; /**< Pitch of the velocity to the target. */
} __attribute__((packed));

/** An Airspeed information message. */
struct airspeedMessage {
  float IAS; /**< Indicated airspeed. */
  float mach; /**< Mach number. */
} __attribute__((packed));

/** A maneuver information message. */
struct maneuverMessage {
  float timeToNextManeuver; /**< Time to the next planned maneuver. */
  float deltaVNextManeuver; /**< Delta to the next planned maneuver. */
  float durationNextManeuver; /**< Duration of the burn for the next planned maneuver. */
  float deltaVTotal; /**< DeltaV of all the planned maneuvers. */
  float headingNextManeuver; /**< Heading of the next maneuver. */
  float pitchNextManeuver; /**< Pitch of the next maneuver. */
} __attribute__((packed));

/** A vessel orientation information message, including velocity orientation. */
struct vesselPointingMessage {
  float heading; /**< Heading of the vessel (between 0 and 360, as indicated in the navball. 90 when pointing east, 180 when pointing south).*/
  float pitch; /**< Pitch angle of the vessel (between -90 and +90. 0 for an horizontal craft, +90 when pointing up, -90 when pointing down). */
  float roll; /**< Roll angle of the vessel (between -180 and +180. 0 for a plane on the runway, ready to takeoff). */
  float orbitalVelocityHeading; /**< Heading of the orbital velocity. */
  float orbitalVelocityPitch; /**< Pitch of the orbital velocity. */
  float surfaceVelocityHeading; /**< Heading of the surface velocity. */
  float surfaceVelocityPitch; /**< Pitch of the surface velocity. */
} __attribute__((packed));

/** A deltaV information message. */
struct deltaVMessage {
  float stageDeltaV; /**< DeltaV of the current stage. */
  float totalDeltaV; /**< DeltaV of the whole vessel. */
} __attribute__((packed));

/** A deltaV information message in different environments.*/
struct deltaVEnvMessage {
	float stageDeltaVASL; /**< DeltaV of the current stage at atmospheric sea level. */
	float totalDeltaVASL; /**< DeltaV of the whole vessel at atmospheric sea level. */
	float stageDeltaVVac; /**< DeltaV of the current stage in vacuum. */
	float totalDeltaVVac; /**< DeltaV of the whole vessel in vacuum. */
} __attribute__((packed));

/** A burn time information message. */
struct burnTimeMessage {
  float stageBurnTime; /**< Burn time of the current stage. */
  float totalBurnTime; /**< Burn time of the whole vessel. */
} __attribute__((packed));

/** A temperator limit message. */
struct tempLimitMessage {
  byte tempLimitPercentage; /**< Maximum temperature percentage (as current temp over max temp) of any part of the vessel. */
  byte skinTempLimitPercentage; /**< Maximum temperature percentage (as current skin temp over max skin temp) of any part of the vessel. */
} __attribute__((packed));

/** A vessel rotation message, only used to send commands to KSP.
    This struct contains information about vessel rotation commands. */
struct rotationMessage {
  int16_t pitch; /**< Vessel pitch. */
  int16_t roll; /**< Vessel roll. */
  int16_t yaw; /**< Vessel yaw. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: pitch (PITCH_ROT)
      - 2: roll (ROLL_ROT)
      - 4: yaw (YAW_ROT)
  */
  byte mask;

  rotationMessage();
  void setPitch(int16_t pitch);
  void setRoll(int16_t roll);
  void setYaw(int16_t yaw);
  void setPitchRollYaw(int16_t pitch, int16_t roll, int16_t yaw);
} __attribute__((packed));

/** A vessel translation message.
    This struct contains information about vessel translation commands. */
struct translationMessage {
  int16_t X; /**< Translation along the X axis. */
  int16_t Y; /**< Translation along the Y axis. */
  int16_t Z; /**< Translation along the Z axis. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: X (X_TRANS)
      - 2: Y (Y_TRANS)
      - 4: Z (Z_TRANS)
  */
  byte mask;

  translationMessage();
  void setX(int16_t x);
  void setY(int16_t y);
  void setZ(int16_t z);
  void setXYZ(int16_t x, int16_t y, int16_t z);
} __attribute__((packed));

/** A vessel custom axis message.
    This struct contains information about custom axis commands. */
struct customAxisMessage {
  int16_t custom1; /**< Value for the custom1 axis. */
  int16_t custom2; /**< Value for the custom2 axis. */
  int16_t custom3; /**< Value for the custom3 axis. */
  int16_t custom4; /**< Value for the custom4 axis. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: Custom1
      - 2: Custom2
      - 4: Custom3
      - 8: Custom4
  */
  byte mask;

  customAxisMessage();
  void setCustom1(int16_t val);
  void setCustom2(int16_t val);
  void setCustom3(int16_t val);
  void setCustom4(int16_t val);
  void setAllCustom(int16_t custom1, int16_t custom2, int16_t custom3, int16_t custom4);
} __attribute__((packed));

/** A wheel control message.
    This struct contains information about wheel steering and throttle. */
struct wheelMessage {
  int16_t steer; /**< Wheel steer. */
  int16_t throttle; /**< Wheel throttle. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: steer (STEER_WHEEL)
      - 2: throttle (THROTTLE_WHEEL)
  */
  byte mask;
  
  wheelMessage();
  void setSteer(int16_t steer);
  void setThrottle(int16_t throttle);
  void setSteerThrottle(int16_t steer, int16_t throttle);
} __attribute__((packed));

/** A throttle control message.
    This struct contains information about throttle. */
struct throttleMessage {
  int16_t throttle; /**< Throttle. */
} __attribute__((packed));


/** A timewarp message.
    This struct contains a single timewarp command, defined in the Timewarp enum. */
struct timewarpMessage {
  byte command; /**< Command in the Timewarp enum. */
} __attribute__((packed));

/** A camera mode message.
    This struct contains information about the camera mode. */
struct cameraRotationMessage {
  int16_t cameraPitch;
  int16_t cameraRoll;
  int16_t cameraYaw;
  int16_t cameraZoom;
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. Based on the RotationAxes enum.
  */
  byte mask;

  cameraRotationMessage();
  void setPitch(int16_t pitch);
  void setRoll(int16_t roll);
  void setYaw(int16_t yaw);
  void setZoom(int16_t zoom);
  void setPitchRollYawZoom(int16_t pitch, int16_t roll, int16_t yaw, int16_t zoom);
} __attribute__((packed));


/** A keyboard emulator message.
    This struct contains information about the keypress to emulate. */
struct keyboardEmulatorMessage {
  byte modifier;
  int16_t keyCode;

  keyboardEmulatorMessage(int16_t keyCode);
  keyboardEmulatorMessage(int16_t keyCode, byte modifier);
} __attribute__((packed));

// Message parsing functions

/** Parse a message of type MsgType.
 *  Use it as 'altitudeMessage myMsg = parseMessage<altitudeMessage>(msg);'
    @param msg The byte array of the message body.
    @returns MsgType A formatted MsgType struct.
*/
template<typename MsgType>
MsgType parseMessage(byte msg[]){
  MsgType* message;
  message = (MsgType*)msg;
  return *message;
}

/** DEPRECATED. Use parseMessage instead. Parse a message containing status of all the CAG.
    @param msg The byte array of the message body.
    @returns cagStatusMessage A formatted cagStatusMessage struct.
*/
cagStatusMessage parseCAGStatusMessage(byte msg[]);

/** DEPRECATED. Use parseMessage instead. Parse a message containing status of the SAS.
    @param msg The byte array of the message body.
    @returns SASInfoMessage A formatted SASInfoMessage struct.
*/
SASInfoMessage parseSASInfoMessage(byte msg[]);

/** DEPRECATED. Use parseMessage instead. Parse a message containing Altitude data.
    @param msg The byte array of the message body.
    @returns altitudeMessage A formatted altitudeMessage struct.
*/
altitudeMessage parseAltitude(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Apsides data.
    @returns apsidesMessage A formatted apsidesMessage struct.
*/
apsidesMessage parseApsides(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing orbital information.
    @returns orbitInfoMessage A formatted orbitInfoMessage struct.
*/
orbitInfoMessage parseOrbitInfo(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Apsides Time data.
    @returns apsidesTimeMessage A formatted apsidesTimeMessage struct.
*/
apsidesTimeMessage parseApsidesTime(byte msg[]);

/** DEPRECATED. Use parseMessage instead. Parse a message countaining Resource data.
    @returns resourceMessage A formatted resourceMessage struct.
*/
resourceMessage parseResource(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message countaining TACLSResourceMessage data.
    @returns TACLSResourceMessage A formatted TACLSResourceMessage struct.
*/
TACLSResourceMessage parseTACLSResource(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message countaining TACLSWasteMessage data.
    @returns TACLSWasteMessage A formatted TACLSWasteMessage struct.
*/
TACLSWasteMessage parseTACLSWaste(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message countaining CustomResourceMessage data.
    @returns CustomResourceMessage A formatted CustomResourceMessage struct.
*/
CustomResourceMessage parseCustomResource(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Velocity data.
    @returns velocityMessage A formatted velocityMessage struct.
*/
velocityMessage parseVelocity(byte msg[]);
/** Parse a message containing Rotation data.
    @returns vesselPointingMessage a formatted vesselPointingMessage struct.
*/
vesselPointingMessage parseRotation(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Target data.
    @returns targetMessage A formatted targetMessage struct.
*/
targetMessage parseTarget(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Airspeed data.
    @returns airspeedMessage a formatted airspeedMessage struct.
*/
airspeedMessage parseAirspeed(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing Maneuver data.
    @returns maneuverMessage a formatted maneuverMessage struct.
*/
maneuverMessage parseManeuver(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing DeltaV data.
    @returns deltaVMessage a formatted deltaVMessage struct.
*/
deltaVMessage parseDeltaV(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing DeltaVEnv data.
    @returns deltaVEnvMessage a formatted deltaVEnvMessage struct.
*/
deltaVEnvMessage parseDeltaVEnv(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing BurnTime data.
    @returns burnTimeMessage a formatted burnTimeMessage struct.
*/
burnTimeMessage parseBurnTime(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing tempLimitMessage data.
    @returns tempLimitMessage a formatted tempLimitMessage struct.
*/
tempLimitMessage parseTempLimitMessage(byte msg[]);
/** DEPRECATED. Use parseMessage instead. Parse a message containing flightStatusMessage data.
    @returns flightStatusMessage a formatted flightStatusMessage struct.
*/
flightStatusMessage parseFlightStatusMessage(byte msg[]);


#endif
