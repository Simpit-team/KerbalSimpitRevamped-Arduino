/** @file KerbalSimPitMessageTypes.h
    Constants for inbound and outbound message IDs.
*/
#ifndef KerbalSimPitMessageTypes_h
#define KerbalSimPitMessageTypes_h

/// Sync message. Used for handshaking.
const byte SYNC_MESSAGE = 0;
/// Echo request. Either end can send this, and an echo response is expected.
const byte ECHO_REQ_MESSAGE = 1;
/// Echo response. Sent in reply to an echo request.
const byte ECHO_RESP_MESSAGE = 2;

/// Scene change packets are sent by the plugin when
/// entering or leaving the flight scene.
const byte SCENE_CHANGE_MESSAGE = 3;
/** Sea level and surface altitude.
    Messages on this channel contain an altitudeMessage.
*/
const byte ALTITUDE_MESSAGE = 8;
/** Apoapsis and periapsis.
    Messages on this channel contain an apsidesMessage.
*/
const byte APSIDES_MESSAGE = 9;

// Resources:
/** Liquid fuel in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte LF_MESSAGE = 10;
/** Liquid fuel in the current stage.
    Messages on this channel contain a resourceMessage. */
const byte LF_STAGE_MESSAGE = 11;
/** Oxidizer in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte OX_MESSAGE = 12;
/** Oxidizer in the current stage.
    Messages on this channel contain a resourceMessage. */
const byte OX_STAGE_MESSAGE = 13;
/** Solid fuel in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte SF_MESSAGE = 14;
/** Solid fuel in the current stage.
    Messages on this channel contain a resourceMessage. */
const byte SF_STAGE_MESSAGE = 15;
/** Monoprollent in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte MONO_MESSAGE = 16;
/** Electic Charge in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte ELECTRIC_MESSAGE = 17;
/** EVA propellant. Only available for Kerbals on EVA.
    Messages on this channel contain a resourceMessage. */
const byte EVA_MESSAGE = 18;
/** Ore in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte ORE_MESSAGE = 19;
/** Ablator in the vessel.
    Messages on this channel contain a resourceMessage. */
const byte AB_MESSAGE = 20;
/** Ablator in the current stage.
    Messages on this channel contain a resourceMessage. */
const byte AB_STAGE_MESSAGE = 21;
/** Vessel velocity.
    Messages on this channel contain a velocityMessage. */
const byte VELOCITY_MESSAGE = 22;

/** Action groups.
    Messages on this channel contain a single byte representing the
    currently active action groups. This could be one or more of

    * STAGE_ACTION
    * GEAR_ACTION
    * LIGHT_ACTION
    * RCS_ACTION
    * SAS_ACTION
    * BRAKES_ACTION
    * ABORT_ACTION
*/
const byte ACTIONSTATUS_MESSAGE = 23;

/** Register to receive messages on a given channel. */
const byte REGISTER_MESSAGE = 8;
/** Deregister, indicate that no further messages
    for the given channel should be sent. */
const byte DEREGISTER_MESSAGE = 9;
// Custom action packets activate and deactivate custom action groups
/** Activate the given Custom Action Group(s). */
const byte CAGACTIVATE_MESSAGE = 10;
/** Deactivate the given Custom Action Group(s). */
const byte CAGDEACTIVATE_MESSAGE = 11;
/** Toggle the given Custom Action Group(s) (Active CAGs will
    deactivate, inactive CAGs will activate). */
const byte CAGTOGGLE_MESSAGE = 12;
// Action group packets activate and deactivate standard action groups
/** Activate the given standard Action Group(s).
    Note that *every request* to activate the Stage action group will
    result in the next stage being activated.
    For all other action groups, multiple activate requests will have
    no effect.
*/
const byte AGACTIVATE_MESSAGE = 13;
/** Deactivate the given standard Action Group(s). */
const byte AGDEACTIVATE_MESSAGE = 14;
/** Toggle the given standard Action Group(s). */
const byte AGTOGGLE_MESSAGE = 15;

/** Bitmask for the Stage action group. */
const byte STAGE_ACTION = 1;
/** Bitmask for the Gear action group. */
const byte GEAR_ACTION = 2;
/** Bitmask for the Light action group. */
const byte LIGHT_ACTION = 4;
/** Bitmask for the RCS action group. */
const byte RCS_ACTION = 8;
/** Bitmask for the SAS action group. */
const byte SAS_ACTION = 16;
/** Bitmask for the Brakes action group. */
const byte BRAKES_ACTION = 32;
/** Bitmask for the Abort action group. */
const byte ABORT_ACTION = 64;
#endif
