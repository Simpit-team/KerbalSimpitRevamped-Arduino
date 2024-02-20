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

    /** Liquid fuel (KSP1) or Methane (KSP2) in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1. */
    LF_MESSAGE = 10,
    /** Liquid fuel (KSP1) or Methane (KSP2) in the current stage.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    LF_STAGE_MESSAGE = 11,
    /** Oxidizer in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    OX_MESSAGE = 12,
    /** Oxidizer in the current stage.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    OX_STAGE_MESSAGE = 13,
    /** Solid fuel in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    SF_MESSAGE = 14,
    /** Solid fuel in the current stage.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    SF_STAGE_MESSAGE = 15,
    /** Xenon gas in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    XENON_GAS_MESSAGE = 28,
    /** Xenon Gas in the current stage.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    XENON_GAS_STAGE_MESSAGE = 29,
    /** Monoprollent in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    MONO_MESSAGE = 16,
    /** EVA propellant. Only works in KSP1. Only available for Kerbals on EVA.
        Messages on this channel contain a resourceMessage. */
    EVA_MESSAGE = 18,

    // |-------------------|
    // | Resources in KSP2 |
    // |-------------------|

    /** Intake air available to the vessel. Only works in KSP2.
         Messages on this channel contain a resourceMessage. */
    INTAKE_AIR_MESSAGE = 52,
    /** Hydrogen in the vessel. Only works in KSP2.
         Messages on this channel contain a resourceMessage.*/
    HYDROGEN_MESSAGE = 53,
    /** Hydrogen in the current Stage. Only works in KSP2.
         Messages on this channel contain a resourceMessage.*/
    HYDROGEN_STAGE_MESSAGE = 54,
    /** Uranium in the vessel. Only works in KSP2.
         Messages on this channel contain a resourceMessage.*/
    URANIUM_MESSAGE = 55,

    //KSP2 offers some more resources, but they don't seem to be in use
    //TEST_ROCKS_MESSAGE = xx,
    //METHALOX_MESSAGE = xx,
    //METHALOX_STAGE_MESSAGE = xx,
    //METHANE_AIR_MESSAGE = xx,
    //METHANE_AIR_STAGE_MESSAGE = xx,
    //XENON_EC_MESSAGE = xx,
    //XENON_EC_STAGE_MESSAGE = xx,

    // |------------------|
    // | Vessel Resources |
    // |------------------|

    /** Electic Charge in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    ELECTRIC_MESSAGE = 17,
    /** Ore in the vessel. Only works in KSP1.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    ORE_MESSAGE = 19,
    /** Ablator in the vessel.
         Messages on this channel contain a resourceMessage. Need ARP to work for KSP1.*/
    AB_MESSAGE = 20,
    /** Ablator in the current stage. Only works in KSP1.
        Messages on this channel contain a resourceMessage. Need ARP to work.*/
    AB_STAGE_MESSAGE = 21,
    /** TAC Life Support ressources. Only works in KSP1. 
        Messages on this channel contain a TACLSRessourceMessage. Need ARP and TACLS to work.*/
    TACLS_RESOURCE_MESSAGE = 30,
    /** TAC Life Support waste ressources. Only works in KSP1. 
        Messages on this channel contain a TACLSWasteMessage. Need ARP and TACLS to work.*/
    TACLS_WASTE_MESSAGE = 31,
	/** Custom ressources. Only works in KSP1. Specific ressource to use to be set in the configuration file.
        Messages on this channel contain a CustomResourceMessage. Need ARP and Community Resource Pack to work.*/
    CUSTOM_RESOURCE_1_MESSAGE = 32,
    /** Custom ressources. Only works in KSP1. Specific ressource to use to be set in the configuration file.
        Messages on this channel contain a CustomResourceMessage. Need ARP and Community Resource Pack to work.*/
    CUSTOM_RESOURCE_2_MESSAGE = 33,

    // |--------------------------|
    // | Vessel Movement/Position |
    // |--------------------------|

    /** Sea level and surface altitude.
        Messages on this channel contain an altitudeMessage. */
    ALTITUDE_MESSAGE = 8,
    /** Vessel velocity.
        Messages on this channel contain a velocityMessage. */
    VELOCITY_MESSAGE = 22,
    /** Information about airspeed.
        This channel delivers messages containing indicated airspeed and
        mach number for the active vessel.
        Messages on this channel contain a airspeedMessage. */
    AIRSPEED_MESSAGE = 27,
    /** Apoapsis and periapsis.
        Messages on this channel contain an apsidesMessage. */
    APSIDES_MESSAGE = 9,
    /** Time to the next apoapsis and periapsis.
        Messages on this channel contain an apsidesTimeMessage. */
    APSIDESTIME_MESSAGE = 24,
    /** Data about the planned maneuvers.
        Messages on this channel contain an maneuverMessage. */
    MANEUVER_MESSAGE = 34,
    /** Data about the current SAS mode.
	    Messages on this channel contain a SASInfoMessage. */
    SAS_MODE_INFO_MESSAGE = 35,
    /** Data about the current orbit.
        Messages on this channel contain an orbitInfoMessage. */
    ORBIT_MESSAGE = 36,
    /** Data about the vessel's rotation (including velocities). This is an output, not be confused with rotationMessage, an input from joysticks.
        Messages on this channel contain an vesselPointingMessage.  
        This value was renamed in v2.2 from ROTATION_DATA. */
    ROTATION_DATA_MESSAGE = 45,


    // |-----------------|
    // | Vessel Commands |
    // |-----------------|

    /** Rotation command after applying Simpit command.
     *  If not Simpit command is applied, this can be set by the SAS for instance.
     *  Messages on this channel contain an rotationMessage, with the mask field unused.
     */
    ROTATION_CMD_MESSAGE = 47,
    /** Translation command after applying Simpit command.
     *  Messages on this channel contain an translationMessage, with the mask field unused.
     */
    TRANSLATION_CMD_MESSAGE = 48,
    /** Wheel command after applying Simpit command.
     *  Messages on this channel contain an wheelMessage, with the mask field unused.
     */
    WHEEL_CMD_MESSAGE = 49,
    /** Throttle command after applying Simpit command.
     *  This can be used for instance to control a motorized throttle.
     *  Messages on this channel contain an throttleMessage (i.e. a single 16-bit integer).
     */
    THROTTLE_CMD_MESSAGE = 50,


    // |----------------|
    // | Vessel Details |
    // |----------------|

    /** Action groups.
        All KSP1 Action Groups are available here, but some KSP2 Action Groups 
        (like Solar and Radiators) are missing. Use ADVANCED_ACTIONSTATUS_MESSAGE for those.
        Messages on this channel contain a single byte representing the
        currently active action groups. A given action group can be checked
        by performing a
        <a href="http://playground.arduino.cc/Code/BitMath#bitwise_and">bitwise AND</a>
        with the message. For example:

        \code
        if (msg[0] & SAS_ACTION) {
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

        The mixed state (some on, some off) from KSP2 is not included.
        Use ADVANCED_ACTIONSTATUS_MESSAGE if you want to see this state.
        "Mixed state" will be false/"off" in this message.
    */
    ACTIONSTATUS_MESSAGE = 37,
    /** Amount of deltaV of the current vessel in the current situation.
        Messages on this channel contain an deltaVMessage. */
    DELTAV_MESSAGE = 38,
    /** Amount of deltaV of the current vessel in different situations
	    (Atmospheric sea level and in vacuum).
        Messages on this channel contain an deltaVEnvMessage. */
    DELTAVENV_MESSAGE = 39,
    /** Amount of burn time of the current vessel.
        Messages on this channel contain an burnTimeMessage. */
    BURNTIME_MESSAGE = 40,
    /** Current status of all the custom action groups.
	    Messages on this channel contains a cagStatusMessage. 
		This cagStatusMessage has a is_action_activated method 
		taking the action group number as an argumetnt. 
        
        The mixed state (some on, some off) from KSP2 is not included. 
        Use ADVANCED_CAGSTATUS_MESSAGE if you want to see this state.
        "Mixed state" will be false/"off" in this message.*/
    CAGSTATUS_MESSAGE = 41,
	/** Current maximum temperature of the any vessel part.
		This message contains the maximum percentage value of the temperature
		of any part of the vessel. It contains a percentage for both core and
		skin temperature. The two maximum percentage can come from different parts.
		Messages on this channel contain an tempLimitMessage. */
    TEMP_LIMIT_MESSAGE = 42,
    /** Default Action groups
        Messages on this channel contain an advancedActionStatusMessage */
    ADVANCED_ACTIONSTATUS_MESSAGE = 56,
    /** Custom Action groups
        Messages on this channel contain an advancedActionStatusMessage */
    ADVANCED_CAGSTATUS_MESSAGE = 57,

    // |----------------------|
    // | External Environment |
    // |----------------------|

    /** Information about targeted object.
        This channel delivers messages about the object targeted by the
        active vessel. Messages on this channel contain a targetInfoMessage. */
    TARGETINFO_MESSAGE = 25,
    /** Name of current Sphere of Influence.
        This channel delivers an ASCII string containing the name of the body
        the active vessel is currently orbiting. Note that this is always the
        English name, regardless of the language the game is currently set to. */
    SOI_MESSAGE = 26,
    /** Scene change packets are sent by the plugin when
     entering or leaving the flight scene.
     This message has a single byte : 0 when a flight is ready, 1 when exiting a flight scene.*/
    SCENE_CHANGE_MESSAGE = 3,
    /** Information about the current flight (warp speed, status, crew, com).
        Messages on this channel contain a flightStatusMessage. */
    FLIGHT_STATUS_MESSAGE = 43,
	/** Information about the current atmospheric conditions.
        Messages on this channel contain a atmoConditionsMessage. */
    ATMO_CONDITIONS_MESSAGE = 44,
	/** Current name of the vessel.
        Messages on this channel contain a table of size 32 max (name is truncated)
        and only ascii characters are used. */
    VESSEL_NAME_MESSAGE = 46,
	/** Vessel Change packets are sent when the current vessel change
        Messages on this channel contain a single byte indicating the origin of the change :
		  1 for vessel switching
		  2 for docking
		  3 for undocking. */
    VESSEL_CHANGE_MESSAGE = 51,
    /** Information about intersects with a target orbit.
        This channel delivers messages about the intersects between the
        active vessel and a target. Messages on this channel contain a intersectsMessage. 
		Only available in KSP2. */
    INTERSECTS_MESSAGE = 60
};

/** Inbound packets.
    These packet types are used for packets going from devices to the game.
*/
enum InboundPackets
{
    /* Packet ID used to ask the computer to close the serial connection to this controller **/
    CLOSE_SERIAL = 7,
    /** Register to receive messages on a given channel. */
    REGISTER_MESSAGE = 8,
    /** Deregister, indicate that no further messages
      for the given channel should be sent. */
    DEREGISTER_MESSAGE = 9,
    /** Request a new message to be sent on this channel (used for channel that only send message on request).
     *  If requested on the channel 0, it will trigger a new message on all subscribed channels. 
     *  May not work on all channels. */
    REQUEST_MESSAGE = 29,
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
    /** Set one of the standard Action Group(s). 
        The payload should be a single byte, 
        with the first 6 bits as which action group to set as in the AdvancedActionGroupIndexes enum
        and the last 2 bits the state/action of the action group as in the ActionGroupSettings enum
       in the AutopilotMode enum.*/
    SETSINGLE_AG_MESSAGE = 58,
    /** Set one of the custom Action Group(s). */
    SETSINGLE_CAG_MESSAGE = 59,
    /** Send vessel rotation commands.
	    If set and not 0, those command will supersede SAS command. The SAS will appear not the work when the jostick is in use.
	    If this is the case for you, make sure you are sending 0 command to KSP on all used axis.*/
    ROTATION_MESSAGE = 16,
    /** Send vessel translation commands.
		Works only for vessels in KSP1. Use keyboard emulation when on EVA.
		Works for vessels and Kerbals on EVA in KSP2. */
    TRANSLATION_MESSAGE = 17,
    /** Send wheel steering/throttle commands. */
    WHEEL_MESSAGE = 18,
    /** Send vessel throttle commands. */
    THROTTLE_MESSAGE = 19,
    /** Send SAS mode commands.
       The payload should be a single byte, possible SAS modes are listed
       in the AutopilotMode enum. */
    SAS_MODE_MESSAGE = 20,
    /** Not yet implemented in KSP2 */
    CAMERA_CONTROL_MODE = 21,
    /** Not yet implemented in KSP2 */
    CAMERA_ROTATION_MESSAGE = 22,

    /** Send a timewarp commands. This is to change the game speed.
       The payload should be a single byte, possible commands are listed
       in the Timewarp enum. */
    TIMEWARP_MESSAGE = 24,
	/** Send a timewarp TO commands. This is to timewarp to a given time.
	   The message (see timewarpToMessage struct) contains a single byte for the specific point to aim (periapsis, maneuver node, etc.)
	   and a delay in seconds with respect to this point.
	   This allow to timewarp 1h before the next morning, 30s before periapsis or 1 min before the start of the burn of the next maneuver.*/
    TIMEWARP_TO_MESSAGE = 30,
	/** Send a custom log message.
	   The message will be printed in the KSP log. The options are defined
	   in the CustomLogStatus enum.
	   The message should not be more than 31 char.*/
    CUSTOM_LOG = 25,
    /** Send a message to emulate a key press.
        The message contains a key modifyer and a key code.
        The key code indicate which key to press. It is taken from the file named VirtualKeyCode
        in the folder KerbalSimpit\Contrib\InputSimulator\WindowsInput\Native.
        For instance :
          - 0x4D for 'm' to open the map
          - 0x74 for 'F5' to quicksave
        The modifier is used to emulate key press while holding keys such as CTRL, SHIFT, etc
        This channel only works on Windows, not on Mac or Linux.

        Another alternative is to use the keyboard.h library, which works on all operating systems
        (Windows, Linux, Mac), but needs a specific Arduino board (based on ATmega32U4 such as the Due or Leonardo).
        See the KerbalSimpitTranslationForShipAndEVADemo example for instance.
     */
    KEYBOARD_EMULATOR = 26,
	/** Send a message to control the custom axis. Only for KSP1 */
    CUSTOM_AXIS_MESSAGE = 27,
    /** Send a message to cycle the NavBall mode to the next mode. */
    NAVBALLMODE_MESSAGE = 28,
};

/** Handshake values
    These are used to communicate the version of KSP the controller is connected to. */
enum HandshakeValues
{
    /** Connection to KSP 1. */
    KERBAL_SPACE_PROGRAM_1 = 0x37,
    /** Connection to KSP 2. */
    KERBAL_SPACE_PROGRAM_2 = 0xC8
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

/** Advanced Action Group Indexes
    These are used to get the values in the ADVANCED_ACTIONSTATUS_MESSAGE. */
enum AdvancedActionGroupIndexes
{
    /** Index for the Stage action group. */
    ADVANCED_STAGE_ACTION = 0,
    /** Index for the Gear action group. */
    ADVANCED_GEAR_ACTION = 1,
    /** Index for the Light action group. */
    ADVANCED_LIGHT_ACTION = 2,
    /** Index for the RCS action group. */
    ADVANCED_RCS_ACTION = 3,
    /** Index for the SAS action group. */
    ADVANCED_SAS_ACTION = 4,
    /** Index for the Brakes action group. */
    ADVANCED_BRAKES_ACTION = 5,
    /** Index for the Abort action group. */
    ADVANCED_ABORT_ACTION = 6,
    /** Index for the Solar action group. Only for KSP2. */
    ADVANCED_SOLAR_ACTION = 7,
    /** Index for the Radiator action group. Only for KSP2. */
    ADVANCED_RADIATOR_ACTION = 8,
    /** Index for the Science action group. Only for KSP2. */
    ADVANCED_SCIENCE_ACTION = 9
};

/** Action Group Settings for single action groups
    These are used to set the values in the SETSINGLE_AG_MESSAGE and SETSINGLE_CAG_MESSAGE. */
enum ActionGroupSettings
{
    /** Don't do anything with this action group. */
    AG_ACTION_NOTHING = 0,
    /** Activate the action group. */
    AG_ACTION_ACTIVATE = 1,
    /** Deactivate the action group. */
    AG_ACTION_DEACTIVATE = 2,
    /** Toggle the action group. */
    AG_ACTION_TOGGLE = 3
};

/** Advanced Action Group States
    These are the values returned by advancedActionStatusMessage::get_action_status in the ADVANCED_ACTIONSTATUS_MESSAGE. */
enum AdvancedActionGroupStates
{
    /** Value if the action group is not available. 
        Use like this: if(myAdvancedActionStatusMessage.getActionStatus(ADVANCED_LIGHT_ACTION) == ADVANCED_AG_STATE_NOT_AVAILABLE) */
    ADVANCED_AG_STATE_NOT_AVAILABLE = 0,
    /** Value if the action group is on. 
        Use like this: if(myAdvancedActionStatusMessage.getActionStatus(ADVANCED_LIGHT_ACTION) == ADVANCED_AG_STATE_ON) */
    ADVANCED_AG_STATE_ON = 1,
    /** Value if the action group is off. 
        Use like this: if(myAdvancedActionStatusMessage.getActionStatus(ADVANCED_LIGHT_ACTION) == ADVANCED_AG_STATE_OFF) */
    ADVANCED_AG_STATE_OFF = 2,
    /** Value if the action group is mixed, meaning some parts associated with it are on and others are off. 
        Use like this: if(myAdvancedActionStatusMessage.getActionStatus(ADVANCED_LIGHT_ACTION) == ADVANCED_AG_STATE_MIXED) */
    ADVANCED_AG_STATE_MIXED = 3,
    /** This bit is on if there is science available, it mirrors the flashing blue light in the UI.
        Use it like this: if((myAdvancedActionStatusMessage.getActionStatus(ADVANCED_SCIENCE_ACTION) & ADVANCED_AG_STATE_BITMASK_SCIENCE_AVAILABLE) != 0) */
    ADVANCED_AG_STATE_BITMASK_SCIENCE_AVAILABLE = 1,
    /** This bit is on if a science experiment is running, the same as when the flask is blinking green.
        Use it like this: if((myAdvancedActionStatusMessage.getActionStatus(ADVANCED_SCIENCE_ACTION) & ADVANCED_AG_STATE_BITMASK_EXPERIMENTS_IN_PROGRESS) != 0) */
    ADVANCED_AG_STATE_BITMASK_EXPERIMENTS_IN_PROGRESS = 2
};


/** Timewarp command
    These are used for a TIMEWARP_MESSAGE. 
    The warp rates given here are for KSP1.
    TIMEWARP_UP, TIMEWARP_DOWN and TIMEWARP_CANCEL_AUTOWARP work for both KSP1 and KSP2
    Use the TIMEWARP_KSP2_XN for setting KSP2 timewarp rates as those rates are different. */
enum Timewarp
{
    /** Set Timewarp to x1. */
    TIMEWARP_X1 = 0,
    /** KSP1: Set Timewarp to x5 (no effect in atmosphere). */
    TIMEWARP_X5 = 1,
    /** KSP1: Set Timewarp to x10 (no effect in atmosphere). */
    TIMEWARP_X10 = 2,
    /** KSP1: Set Timewarp to x50 (no effect in atmosphere). */
    TIMEWARP_X50 = 3,
    /** KSP1: Set Timewarp to x100 (no effect in atmosphere). */
    TIMEWARP_X100 = 4,
    /** KSP1: Set Timewarp to x1000 (no effect in atmosphere). */
    TIMEWARP_X1000 = 5,
    /** KSP1: Set Timewarp to x10000 (no effect in atmosphere). */
    TIMEWARP_X10000 = 6,
    /** KSP1: Set Timewarp to x100000 (no effect in atmosphere). */
    TIMEWARP_X100000 = 7,
    /** KSP1: Set Timewarp to x1 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X1_PHYSICAL = 8,
    /** KSP1: Set Timewarp to x2 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X2_PHYSICAL = 9,
    /** KSP1: Set Timewarp to x3 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X3_PHYSICAL = 10,
    /** KSP1: Set Timewarp to x4 in atmosphere (no effect out of atmosphere). */
    TIMEWARP_X4_PHYSICAL = 11,
    /** KSP1 and KSP2: Set Timewarp the next rate available. */
    TIMEWARP_UP = 12,
    /** KSP1 and KSP2: Set Timewarp the previous rate available. */
    TIMEWARP_DOWN = 13,
    /** KSP1 and KSP2: Cancel the current auto-timewarp and reset it to x1. */
    TIMEWARP_CANCEL_AUTOWARP = 255
};

/** Timewarp command
    These are used for a TIMEWARP_MESSAGE.
    The warp rates given here are for KSP2.
    Use the TIMEWARP_XN for setting KSP1 timewarp rates as those rates are different.
    TIMEWARP_UP, TIMEWARP_DOWN and TIMEWARP_CANCEL_AUTOWARP from the KSP1 set of Timewarp rates work for both KSP1 and KSP2. */
enum TimewarpKsp2
{
    /** KSP2: Set Timewarp to x1. */
    TIMEWARP_KSP2_X1 = 0,
    /** KSP2: SSet Timewarp to x2. */
    TIMEWARP_KSP2_X2 = 1,
    /** KSP2: SSet Timewarp to x4. */
    TIMEWARP_KSP2_X4 = 2,
    /** KSP2: SSet Timewarp to x10. */
    TIMEWARP_KSP2_X10 = 3,
    /** KSP2: SSet Timewarp to x50. */
    TIMEWARP_KSP2_X50 = 4,
    /** KSP2: SSet Timewarp to x100. */
    TIMEWARP_KSP2_X100 = 5,
    /** KSP2: SSet Timewarp to x1000. */
    TIMEWARP_KSP2_X1000 = 6,
    /** KSP2: SSet Timewarp to x10000. */
    TIMEWARP_KSP2_X10000 = 7,
    /** KSP2: SSet Timewarp to x100 000. */
    TIMEWARP_KSP2_X100000 = 8,
    /** KSP2: SSet Timewarp to x1 000 000. */
    TIMEWARP_KSP2_X1000000 = 9,
    /** KSP2: SSet Timewarp to x10 000 000 */
    TIMEWARP_KSP2_X10000000 = 10
};

/** Timewarp command
    These are used for a TIMEWARP_TO_MESSAGE in a timewarpToMessage struct. */
enum TimewarpTo {
	/** Warp to now, used to add a delay to the given time */
	TIMEWARP_TO_NOW = 0,
	/** Warp to the next maneuver node. */
    TIMEWARP_TO_NEXT_MANEUVER = 1,
	/** Warp to the start of the burn of the next maneuver, estiming the burntime using KSP estimations. It is recommended to add a delay to account for the needed rotation. */
    TIMEWARP_TO_NEXT_BURN = 2,
    /** Warp to the next SOI change. */
    TIMEWARP_TO_NEXT_SOI = 3,
    /** Warp to the apoapsis. */
    TIMEWARP_TO_APOAPSIS = 4,
    /** Warp to the periapsis. */
    TIMEWARP_TO_PERIAPSIS = 5,
    /** Warp to the next morning. Not implemented in KSP2 yet. */
    TIMEWARP_TO_NEXT_MORNING = 6,
};

enum CustomLogStatus {
    /** If set, the message will only be put in the KSP log is the Simpit mod is in verbose mode */
    VERBOSE_ONLY = 1,
    /** If set, the message will also be displayed to the user (and not only in the log) */
    PRINT_TO_SCREEN = 2,
    /** If set, the message will *not* be prefixed with 'Simpit :' */
    NO_HEADER = 4,
};

enum KeyboardEmulatorModifier {
  /** If set, emulate the use of Shift */
  SHIFT_MOD = 1,
  /** If set, emulate the use of CTRL */
  CTRL_MOD = 2,
  /** If set, emulate the use of ALT */
  ALT_MOD = 4,
  /** If set, emulate only the key down */
  KEY_DOWN_MOD = 8,
  /** If set, emulate only the key up */
  KEY_UP_MOD = 16,
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
    VesselPilot.AutopilotMode in the KSP1 API (values 0 to 9)
    and with KSP.Sim.AutopilotMode in KSP2 (values 0 to 11) */
enum AutopilotMode
{
    AP_STABILITYASSIST = 0,
    AP_PROGRADE = 1,
    AP_RETROGRADE = 2,
    AP_NORMAL = 3,
    AP_ANTINORMAL = 4,
    AP_RADIALIN = 5,
    AP_RADIALOUT = 6,
    AP_TARGET = 7,
    AP_ANTITARGET = 8,
    AP_MANEUVER = 9,
    AP_NAVIGATION = 10,
    AP_AUTOPILOT = 11
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

    CAMERA_NEXT = 50, //unused
    CAMERA_PREVIOUS = 51, //unused
    CAMERA_NEXT_MODE = 52,
    CAMERA_PREVIOUS_MODE = 53

};

/** Flags used for the FlightStatus message. */
enum FligthStatusFlags
{
    FLIGHT_IN_FLIGHT = 1, /**< True if the game is currently in the flight screen. */
    FLIGHT_IS_EVA = 2, /**< True if the current flight is an EVA. */
    FLIGHT_IS_RECOVERABLE = 4, /**< True if the current vesset is recoverable. */
    FLIGHT_IS_ATMO_TW = 8, /**< True if the current Timewarp mode is for atmosphere (i.e. physical timewarp). */
    FLIGHT_CONTROL_LEVEL_0 = 16, /**< 2 bits used to code the Control Level. Use the helper function of flightStatusMessage to parse it. */
    FLIGHT_CONTROL_LEVEL_1 = 32, /**< 2 bits used to code the Control Level. Use the helper function of flightStatusMessage to parse it. */
    FLIGHT_HAS_TARGET = 64, /**< True if a target is set currently. */
};

/** Flags used for the atmoConditionsMessage message. */
enum AtmoConditionsFlags
{
    HAS_ATMOSPHERE = 1, /**< True if the current body has an atmosphere. */
    HAS_OXYGEN = 2, /**< True if the current body has oxygen in its atmosphere. */
    IS_IN_ATMOSPHERE = 4, /**< True if the current vesset is atmosphere. */
};

#endif
