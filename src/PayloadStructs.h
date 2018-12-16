/** @file PayloadStructs.h
    Structs for compound message types.
*/
#ifndef PayloadStructs_h
#define PayloadStructs_h

#include <Arduino.h>

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

/** A Resource message.
    All resource messages use this struct for sending data. */
struct resourceMessage {
  float total; /**< Maximum capacity of the resource. */
  float available; /**< Current resource level. */
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
} __attribute__((packed));

/** An Airspeed information message. */
struct airspeedMessage {
  float IAS; /**< Indicated airspeed. */
  float mach; /**< Mach number. */
} __attribute__((packed));

/** A vessel rotation message.
    This struct contains information about vessel rotation commands. */
struct rotationMessage {
  int16_t pitch; /**< Vessel pitch. */
  int16_t roll; /**< Vessel roll. */
  int16_t yaw; /**< Vessel yaw. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: pitch
      - 2: roll
      - 4: yaw
  */
  byte mask;
} __attribute__((packed));

/** A vessel translation message.
    This struct contains information about vessel translation commands. */
struct translationMessage {
  int16_t X; /**< Translation along the X axis. */
  int16_t Y; /**< Translation along the Y axis. */
  int16_t Z; /**< Translation along the Z axis. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: X
      - 2: Y
      - 4: Z
  */
  byte mask;
} __attribute__((packed));

/** A wheel control message.
    This struct contains information about wheel steering and throttle. */
struct wheelMessage {
  int16_t steer; /**< Wheel steer. */
  int16_t throttle; /**< Wheel throttle. */
  /** The mask indicates which elements are intentionally set. Unset elements
      should be ignored. It should be one or more of:

      - 1: steer
      - 2: throttle
  */
  byte mask;
} __attribute__((packed));

// Message parsing functions

/** Parse a message containing Altitude data.
    @param msg The byte array of the message body.
    @returns altitudeMessage A formatted altitudeMessage struct.
*/
altitudeMessage parseAltitude(byte msg[]);
/** Parse a message containing Apsides data.
    @returns apsidesMessage A formatted apsidesMessage struct.
*/
apsidesMessage parseApsides(byte msg[]);

/** Parse a message containing Apsides Time data.
    @returns apsidesTimeMessage A formatted apsidesTimeMessage struct.
*/
apsidesTimeMessage parseApsidesTime(byte msg[]);

/** Parse a message countaining Resource data.
    @returns resourceMessage A formatted resourceMessage struct.
*/
resourceMessage parseResource(byte msg[]);
/** Parse a message containing Velocity data.
    @returns velocityMessage A formatted velocityMessage struct.
*/
velocityMessage parseVelocity(byte msg[]);
/** Parse a message containing Target data.
    @returns targetMessage A formatted targetMessage struct.
*/
targetMessage parseTarget(byte msg[]);
/** Parse a message containing Airspeed data.
    @returns airspeedMessage a formatted airspeedMessage struct.
*/
airspeedMessage parseAirspeed(byte msg[]);

#endif
