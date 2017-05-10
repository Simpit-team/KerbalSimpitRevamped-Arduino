#ifndef KerbalSimPitMessageTypes_h
#define KerbalSimPitMessageTypes_h

// These are the recognised packet types
// Sync packets are used for handshaking
const byte SYNC_MESSAGE = 0;
// Echo request packet. Either end can send this.
const byte ECHO_REQ_MESSAGE = 1;
// Echo response packet. Sent in reply to an echo request.
const byte ECHO_RESP_MESSAGE = 2;

// Scene change packets are sent by the game when
// entering or leaving the flight scene.
const byte SCENE_CHANGE_MESSAGE = 3;
// Altitude packets indicate sea level and surface altitude
const byte ALTITUDE_MESSAGE = 8;
// Apsides packets indicate apoapsis and periapsis
const byte APSIDES_MESSAGE = 9;
// Resources:
// Total and stage liquid fuel
const byte LF_MESSAGE = 10;
const byte LF_STAGE_MESSAGE = 11;
// Total and stage oxidizer
const byte OX_MESSAGE = 12;
const byte OX_STAGE_MESSAGE = 13;
// Total and stage solid fuel
const byte SF_MESSAGE = 14;
const byte SF_STAGE_MESSAGE = 15;
// Total monopropellant
const byte MONO_MESSAGE = 16;
// Total electric charge
const byte ELECTRIC_MESSAGE = 17;
// Total Eva propellant
const byte EVA_MESSAGE = 18;
// Total Ore
const byte ORE_MESSAGE = 19;
// Total and stage ablator
const byte AB_MESSAGE = 20;
const byte AB_STAGE_MESSAGE = 21;

// Register and deregister packets are sent by a device to register
// or deregister to event channels.
const byte REGISTER_MESSAGE = 8;
const byte DEREGISTER_MESSAGE = 9;
// Custom action packets activate and deactivate custom action groups
const byte CAGACTIVATE_MESSAGE = 10;
const byte CAGDEACTIVATE_MESSAGE = 11;
const byte CAGTOGGLE_MESSAGE = 12;
// Action group packets activate and deactivate standard action groups
const byte AGACTIVATE_MESSAGE = 13;
const byte AGDEACTIVATE_MESSAGE = 14;
const byte AGTOGGLE_MESSAGE = 15;

// These bytes are payloads for the different types of actions
const byte STAGE_ACTION = 1;
const byte GEAR_ACTION = 2;
const byte LIGHT_ACTION = 4;
const byte RCS_ACTION = 8;
const byte SAS_ACTION = 16;
const byte BRAKES_ACTION = 32;
const byte ABORT_ACTION = 64;
#endif
