/** @file KerbalSimpitMessageTypes.h
    Constants for inbound and outbound message IDs.
*/
#ifndef KerbalSimpitMessageTypes_h
#define KerbalSimpitMessageTypes_h

/** Common packets.
    These packet types are used for both inbound and outbound messages.
*/
enum CommonPackets
{
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
enum OutboundPackets
{

    // |----------------------|
    // | Propulsion Resources |
    // |----------------------|

    /** Liquid fuel in the vessel.
         Messages on this channel contain a resourceMessage. */
    LF_MESSAGE = 1,
    /** Liquid fuel in the current stage.
         Messages on this channel contain a resourceMessage. */
    LF_STAGE_MESSAGE = 2,
    /** Oxidizer in the vessel.
         Messages on this channel contain a resourceMessage. */
    OX_MESSAGE = 3,
    /** Oxidizer in the current stage.
         Messages on this channel contain a resourceMessage. */
    OX_STAGE_MESSAGE = 4,
    /** Solid fuel in the vessel.
         Messages on this channel contain a resourceMessage. */
    SF_MESSAGE = 5,
    /** Solid fuel in the current stage.
         Messages on this channel contain a resourceMessage. */
    SF_STAGE_MESSAGE = 6,
    /** Xenon gas in the vessel.
         Messages on this channel contain a resourceMessage. */
    XENON_GAS_MESSAGE = 7,
    /** Xenon Gas in the current stage.
         Messages on this channel contain a resourceMessage. */
    XENON_GAS_STAGE_MESSAGE = 8,
    /** Monoprollent in the vessel.
         Messages on this channel contain a resourceMessage. */
    MONO_MESSAGE = 9,
    /** EVA propellant. Only available for Kerbals on EVA.
        Messages on this channel contain a resourceMessage. */
    EVA_MESSAGE = 10,

    // |------------------|
    // | Vessel Resources |
    // |------------------|

    /** Electic Charge in the vessel.
         Messages on this channel contain a resourceMessage. */
    ELECTRIC_MESSAGE = 20,
    /** Ore in the vessel.
         Messages on this channel contain a resourceMessage. */
    ORE_MESSAGE = 21,
    /** Ablator in the vessel.
         Messages on this channel contain a resourceMessage. */
    AB_MESSAGE = 22,
    /** Ablator in the current stage.
        Messages on this channel contain a resourceMessage. */
    AB_STAGE_MESSAGE = 23,

    // |--------------------------|
    // | Vessel Movement/Position |
    // |--------------------------|

    /** Sea level and surface altitude.
        Messages on this channel contain an altitudeMessage. */
    ALTITUDE_MESSAGE = 30,
    /** Vessel velocity.
        Messages on this channel contain a velocityMessage. */
    VELOCITY_MESSAGE = 31,
    /** Information about airspeed.
        This channel delivers messages containing indicated airspeed and
        mach number for the active vessel. */
    AIRSPEED_MESSAGE = 32,
    /** Apoapsis and periapsis.
        Messages on this channel contain an apsidesMessage. */
    APSIDES_MESSAGE = 33,
    /** Time to the next apoapsis and periapsis.
        Messages on this channel contain an apsidesTimeMessage. */
    APSIDESTIME_MESSAGE = 34,
    /** Data about the planned maneuvers.
        Messages on this channel contain an maneuverMessage. */
    MANEUVER_MESSAGE = 35,

    SAS_MODE_STATUS_MESSAGE = 36,
    /** Data about the current orbit.
        Messages on this channel contain an orbitInfoMessage. */
    ORBIT_INFO = 37,

    // |----------------|
    // | Vessel Details |
    // |----------------|

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
    ACTIONSTATUS_MESSAGE = 40,
    /** Amount of deltaV of the current vessel in the current situation.
        Messages on this channel contain an deltaVMessage. */
    DELTAV_MESSAGE = 41,
    /** Amount of deltaV of the current vessel in different situations
	    (Atmospheric sea level and in vacuum).
        Messages on this channel contain an deltaVEnvMessage. */
    DELTAVENV_MESSAGE = 42,
    /** Amount of burn time of the current vessel.
        Messages on this channel contain an burnTimeMessage. */
    BURNTIME_MESSAGE = 43,
    /** Current status of all the custom action groups.
	    Messages on this channel contains a cagStatusMessage. 
		This cagStatusMessage has a is_action_activated method 
		taking the action ground number as an argumetnt. */
    CAGSTATUS_MESSAGE = 44,

    // |----------------------|
    // | External Environment |
    // |----------------------|

    /** Information about targetted object.
        This channel delivers messages about the object targetted by the
        active vessel. Messages on this channel contain a targetInfoMessage. */
    TARGETINFO_MESSAGE = 50,
    /** Name of current Sphere of Influence.
        This channel delivers an ASCII string containing the name of the body
        the active vessel is currently orbiting. Note that this is always the
        English name, regardless of the language the game is currently set to. */
    SOI_MESSAGE = 51,
    /** Scene change packets are sent by the plugin when
     entering or leaving the flight scene. */
    SCENE_CHANGE_MESSAGE = 52

};

/** Inbound packets.
    These packet types are used for packets going from devices to the game.
*/
enum InboundPackets
{
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
    SAS_MODE_MESSAGE = 20,
    CAMERA_CONTROL_MODE = 21,
    CAMERA_ROTATION_MESSAGE = 22,

    /** Send a time warp commands.
       The payload should be a single byte, possible commands are listed
       in the Timewarp enum. */
    TIMEWARP_MESSAGE = 24,
	/** Send a custom log message.
	   The message will be printed in the KSP log. The options are defined
	   in the CustomLogStatus enum.
	   The message should not be more than 31 char.*/
    CUSTOM_LOG = 25,
};

/** Action Group Indexes
    These are used to mask out elements of an ACTIONSTATUS_MESSAGE. */
enum ActionGroupIndexes
{
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

/** Timewarp command
    These are used for a TIMEWARP_MESSAGE. */
enum Timewarp
{
    /** Set Timewarp to x1. */
    TIMEWARP_X1 = 0,
    /** Set Timewarp to x5 (no effect in atmosphere). */
    TIMEWARP_X5 = 1,
    /** Set Timewarp to x10 (no effect in atmosphere). */
    TIMEWARP_X10 = 2,
    /** Set Timewarp to x50 (no effect in atmosphere). */
    TIMEWARP_X50 = 3,
    /** Set Timewarp to x100 (no effect in atmosphere). */
    TIMEWARP_X100 = 4,
    /** Set Timewarp to x1000 (no effect in atmosphere). */
    TIMEWARP_X1000 = 5,
    /** Set Timewarp to x10000 (no effect in atmosphere). */
    TIMEWARP_X10000 = 6,
    /** Set Timewarp to x100000 (no effect in atmosphere). */
    TIMEWARP_X100000 = 7,
    /** Set Timewarp to x1 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X1_PHYSICAL = 8,
    /** Set Timewarp to x2 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X2_PHYSICAL = 9,
    /** Set Timewarp to x3 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X3_PHYSICAL = 10,
    /** Set Timewarp to x4 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X4_PHYSICAL = 11,
    /** Set Timewarp the next rate available. */
    TIMEWARP_UP = 12,
    /** Set Timewarp the previous rate available. */
    TIMEWARP_DOWN = 13,
    /** Warp to the next maneuver. */
    TIMEWARP_NEXT_MANEUVER = 14,
    /** Warp to the next SOI change. */
    TIMEWARP_NEXT_SOI = 15,
    /** Warp to the apoapsis. */
    TIMEWARP_APOAPSIS = 16,
    /** Warp to the periapsis. */
    TIMEWARP_PERIAPSIS = 17,
    /** Warp to the next morning. */
    TIMEWARP_NEXT_MORNING = 18,
    /** Cancel the current auto-timewarp and reset it to x1. */
    TIMEWARP_CANCEL_AUTOWARP = 255
};

enum CustomLogStatus {
    /** If set, the message will only be put in the KSP log is the Simpit mod is in verbose mode */
    VERBOSE_ONLY = 1,
    /** If set, the message will also be displayed to the user (and not only in the log) */
    PRINT_TO_SCREEN = 2,
    /** If set, the message will *not* be prefixed with 'Simpit :' */
    NO_HEADER = 4,
};

/** Rotation Axes
    These are used to indicate which axes in a ROTATION_MESSAGE or CAMERA_ROTATION_MESSAGE are active. */
enum RotationAxes {
  PITCH_ROT = 1, /**< Bitmask for the pitch axis. */
  ROLL_ROT = 2, /**< Bitmask for the roll axis. */
  YAW_ROT = 4, /**< Bitmask for the yaw axis. */
  ZOOM_ROT = 8 /**< Bitmask for the zoom axis. Added here purely for camera control. */
};

/** Translation Axes
    These are used to indicate which axes in a TRANSLATION_MESSAGE are active. */
enum TranslationAxes
{
    X_TRANS = 1, /**< Bitmask for the X axis. */
    Y_TRANS = 2, /**< Bitmask for the Y axis. */
    Z_TRANS = 4  /**< Bitmask for the Z axis. */
};

/** Translation Axes
    These are used to indicate which axes in a WHEEL_MESSAGE are active. */
enum WheelAxes {
  STEER_WHEEL = 1, /**< Bitmask for the steer. */
  THROTTLE_WHEEL = 2, /**< Bitmask for the throttle. */
};

/** Autopilot Mode
    The possible Autopilot (SAS) modes. This enum corresponds with
    VesselPilot.AutopilotMode in the KSP API. */
enum AutopilotMode
{
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

/** Camera Mode
    The possible camera mode control options. */
enum CameraControlMode
{

    // Flight Camera Modes
    CAMERA_MODE_FLIGHT = 1,

    FLIGHT_CAMERA_AUTO = 2,
    FLIGHT_CAMERA_FREE = 3,
    FLIGHT_CAMERA_ORBITAL = 4,
    FLIGHT_CAMERA_CHASE = 5,
    FLIGHT_CAMERA_LOCKED = 6,

    CAMERA_NEXT = 50,
    CAMERA_PREVIOUS = 51,
    CAMERA_NEXT_MODE = 52,
    CAMERA_PREVIOUS_MODE = 53

};

#endif
