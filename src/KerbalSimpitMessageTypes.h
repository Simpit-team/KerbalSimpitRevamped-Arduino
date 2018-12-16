/** @file KerbalSimpitMessageTypes.h
    Constants for inbound and outbound message IDs.
*/
#ifndef KerbalSimpitMessageTypes_h
#define KerbalSimpitMessageTypes_h

/** Common packets.
    These packet types are used for both inbound and outbound messages.
*/
enum CommonPackets {
  /// Sync message. Used for handshaking.
  SYNC_MESSAGE = 0,
  /// Echo request. Either end can send this, and an echo response is expected.
  ECHO_REQ_MESSAGE = 1,
  /// Echo response. Sent in reply to an echo request.
  ECHO_RESP_MESSAGE = 2
};

/** Outbound packets.
    IDs for packets that go from the game to devices.
*/
enum OutboundPackets {
  /// Scene change packets are sent by the plugin when
  /// entering or leaving the flight scene.
  SCENE_CHANGE_MESSAGE = 3,
  /** Sea level and surface altitude.
      Messages on this channel contain an altitudeMessage.
  */
  ALTITUDE_MESSAGE = 8,
  /** Apoapsis and periapsis.
      Messages on this channel contain an apsidesMessage.
  */
  APSIDES_MESSAGE = 9,

  // Resources:
  /** Liquid fuel in the vessel.
      Messages on this channel contain a resourceMessage. */
  LF_MESSAGE = 10,
  /** Liquid fuel in the current stage.
      Messages on this channel contain a resourceMessage. */
  LF_STAGE_MESSAGE = 11,
  /** Oxidizer in the vessel.
      Messages on this channel contain a resourceMessage. */
  OX_MESSAGE = 12,
  /** Oxidizer in the current stage.
      Messages on this channel contain a resourceMessage. */
  OX_STAGE_MESSAGE = 13,
  /** Solid fuel in the vessel.
      Messages on this channel contain a resourceMessage. */
  SF_MESSAGE = 14,
  /** Solid fuel in the current stage.
      Messages on this channel contain a resourceMessage. */
  SF_STAGE_MESSAGE = 15,
  /** Monoprollent in the vessel.
      Messages on this channel contain a resourceMessage. */
  MONO_MESSAGE = 16,
  /** Electic Charge in the vessel.
      Messages on this channel contain a resourceMessage. */
  ELECTRIC_MESSAGE = 17,
  /** EVA propellant. Only available for Kerbals on EVA.
      Messages on this channel contain a resourceMessage. */
  EVA_MESSAGE = 18,
  /** Ore in the vessel.
      Messages on this channel contain a resourceMessage. */
  ORE_MESSAGE = 19,
  /** Ablator in the vessel.
      Messages on this channel contain a resourceMessage. */
  AB_MESSAGE = 20,
  /** Ablator in the current stage.
      Messages on this channel contain a resourceMessage. */
  AB_STAGE_MESSAGE = 21,
  /** Vessel velocity.
      Messages on this channel contain a velocityMessage. */
  VELOCITY_MESSAGE = 22,

  /** Action groups.
      Messages on this channel contain a single byte representing the
      currently active action groups. A given action group can be checked
      by performing a
      <a href="http://playground.arduino.cc/Code/BitMath#bitwise_and">bitwise AND</a>
      with the message. For example:

      \code
      if (msg & SAS_ACTION) {
        // code to execute if SAS is active
      }
      \endcode

      Possible action groups are:

      - STAGE_ACTION
      - GEAR_ACTION
      - LIGHT_ACTION
      - RCS_ACTION
      - SAS_ACTION
      - BRAKES_ACTION
      - ABORT_ACTION 
  */
  ACTIONSTATUS_MESSAGE = 23,

  /** Time to the next apoapsis and periapsis.
      Messages on this channel contain an apsidesTimeMessage. */
  APSIDESTIME_MESSAGE = 24,

  /** Information about targetted object.
      This channel delivers messages about the object targetted by the
      active vessel. Messages on this channel contain a targetInfoMessage. */
  TARGETINFO_MESSAGE = 25,

  /** Name of current Sphere of Influence.
      This channel delivers an ASCII string containing the name of the body
      the active vessel is currently orbiting. Note that this is always the
      English name, regardless of the language the game is currently set to. */
  SOI_MESSAGE = 26,

  /** Information about airspeed.
      This channel delivers messages containing indicated airspeed and
      mach number for the active vessel. */
  AIRSPEED_MESSAGE = 27,
};

/** Inbound packets.
    These packet types are used for packets going from devices to the game.
*/
enum InboundPackets {
  /** Register to receive messages on a given channel. */
  REGISTER_MESSAGE = 8,
  /** Deregister, indicate that no further messages
      for the given channel should be sent. */
  DEREGISTER_MESSAGE = 9,
  // Custom action packets activate and deactivate custom action groups
  /** Activate the given Custom Action Group(s). */
  CAGACTIVATE_MESSAGE = 10,
  /** Deactivate the given Custom Action Group(s). */
  CAGDEACTIVATE_MESSAGE = 11,
  /** Toggle the given Custom Action Group(s) (Active CAGs will
      deactivate, inactive CAGs will activate). */
  CAGTOGGLE_MESSAGE = 12,
  /** Activate the given standard Action Group(s).
      Note that *every request* to activate the Stage action group will
      result in the next stage being activated.
      For all other action groups, multiple activate requests will have
      no effect.
  */
  AGACTIVATE_MESSAGE = 13,
  /** Deactivate the given standard Action Group(s). */
  AGDEACTIVATE_MESSAGE = 14,
  /** Toggle the given standard Action Group(s). */
  AGTOGGLE_MESSAGE = 15,
  /** Send vessel rotation commands. */
  ROTATION_MESSAGE = 16,
  /** Send vessel translation commands. */
  TRANSLATION_MESSAGE = 17,
  /** Send wheel steering/throttle commands. */
  WHEEL_MESSAGE = 18,
  /** Send vessel throttle commands. */
  THROTTLE_MESSAGE = 19,
  /** Send SAS mode commands.
       The payload should be a single byte, possible SAS modes are listed
       in the AutopilotMode enum. */
  SAS_MODE_MESSAGE = 20
};

/** Action Group Indexes
    These are used to mask out elements of an ACTIONSTATUS_MESSAGE. */
enum ActionGroupIndexes {
  /** Bitmask for the Stage action group. */
  STAGE_ACTION = 1,
  /** Bitmask for the Gear action group. */
  GEAR_ACTION = 2,
  /** Bitmask for the Light action group. */
  LIGHT_ACTION = 4,
  /** Bitmask for the RCS action group. */
  RCS_ACTION = 8,
  /** Bitmask for the SAS action group. */
  SAS_ACTION = 16,
  /** Bitmask for the Brakes action group. */
  BRAKES_ACTION = 32,
  /** Bitmask for the Abort action group. */
  ABORT_ACTION = 64
};

/** Rotation Axes
    These are used to indicate which axes in a ROTATION_MESSAGE are active. */
enum RotationAxes {
  PITCH_ROT = 1, /**< Bitmask for the pitch axis. */
  ROLL_ROT = 2, /**< Bitmask for the roll axis. */
  YAW_ROT = 4 /**< Bitmask for thw yaw axis. */
};

/** Translation Axes
    These are used to indicate which axes in a TRANSLATION_MESSAGE are active. */
enum TranslationAxes {
  X_TRANS = 1, /**< Bitmask for the X axis. */
  Y_TRANS = 2, /**< Bitmask for the Y axis. */
  Z_TRANS = 4 /**< Bitmask for the Z axis. */
};

/** Autopilot Mode
    The possible Autopilot (SAS) modes. This enum corresponds with
    VesselPilot.AutopilotMode in the KSP API. */
enum AutopilotMode {
  AP_STABILITYASSIST = 1,
  AP_PROGRADE = 2,
  AP_RETROGRADE = 3,
  AP_NORMAL = 4,
  AP_ANTINORMAL = 5,
  AP_RADIALIN = 6,
  AP_RADIALOUT = 7,
  AP_TARGET = 8,
  AP_ANTITARGET = 9,
  AP_MANEUVER = 10
};

#endif
