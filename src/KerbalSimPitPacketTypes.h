#ifndef KerbalSimPitPacketTypes_h
#define KerbalSimPitPacketTypes_h

// These are the recognised packet types
// Sync packets are used for handshaking
const byte SYNC_PACKET = 0;
// Echo request packet. Either end can send this.
const byte ECHO_REQ_PACKET = 1;
// Echo response packet. Sent in reply to an echo request.
const byte ECHO_RESP_PACKET = 2;

// Scene change packets are sent by the game when
// entering or leaving the flight scene.
const byte SCENE_CHANGE_PACKET = 3;
// Altitude packets indicate sea level and surface altitude
const byte ALTITUDE_PACKET = 4;

// Register and deregister packets are sent by a device to register
// or deregister to event channels.
const byte REGISTER_PACKET = 3;
const byte DEREGISTER_PACKET = 4;
// Custom action packets activate and deactivate custom action groups
const byte CAGACTIVATE_PACKET = 6;
const byte CAGDEACTIVATE_PACKET = 7;
const byte CAGTOGGLE_PACKET = 8;
// Action group packets activate and deactivate standard action groups
const byte AGACTIVATE_PACKET = 9;
const byte AGDEACTIVATE_PACKET = 10;
const byte AGTOGGLE_PACKET = 11;

// These bytes are payloads for the different types of actions
const byte STAGE_ACTION = 1;
const byte GEAR_ACTION = 2;
const byte LIGHT_ACTION = 4;
const byte RCS_ACTION = 8;
const byte SAS_ACTION = 16;
const byte BRAKES_ACTION = 32;
const byte ABORT_ACTION = 64;
#endif
