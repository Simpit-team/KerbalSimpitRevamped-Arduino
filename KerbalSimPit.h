#ifndef KerbalSimPit_h
#define KerbalSimPit_h

#include <Arduino.h>

// These are the recognised packet types
// Sync packets are used for handshaking
const byte SYNC_PACKET = 0x00;
// Echo request packet. Either end can send this.
const byte ECHO_REQ_PACKET = 0x01;
// Echo response packet. Sent in reply to an echo request.
const byte ECHO_RESP_PACKET = 0x02;

// Scene change packets are sent by the game when
// entering or leaving the flight scene.
const byte SCENE_CHANGE_PACKET = 0x03;
// Altitude packets indicate sea level and surface altitude
const byte ALTITUDE_PACKET = 0x04;

// Register and deregister packets are sent by a device to register
// or deregister to event channels.
const byte REGISTER_PACKET = 0x03;
const byte DEREGISTER_PACKET = 0x04;
// Stage packet activates the next stage.
const byte STAGE_PACKET = 0x05;
// Custom action packets activate and deactivate custom action groups
const byte CAGACTIVATE_PACKET = 0x06;
const byte CAGDEACTIVATE_PACKET = 0x07;

class KerbalSimPit
{
 public:
  KerbalSimPit(Stream *serial);
  bool init();
  void inboundHandler(void (*packetHandler)(byte packetType,
                                            byte *msg, byte msgSize));
  void registerChannel(byte channelID);
  void deregisterChannel(byte channelID);
  void send(byte packetType, byte *msg, byte msgSize);
  void update();

  // Helper functions
  void stageEvent();
  void activateCAG(byte *msg);
  void deactivateCAG(byte *msg);

 private:
  byte _inboundType;
  byte _inboundSize;
  byte _inboundBuffer[32];
  byte _outboundBuffer[32];
  byte _outboundSize;
  Stream *_serial;

  enum ReceiveState_t
  {
    WaitingFirstByte,
    WaitingSecondByte,
    WaitingSize,
    WaitingType,
    WaitingData,
  };
  ReceiveState_t _receiveState;
  byte _receivedIndex;

  void (*_packetHandler)(byte packetType, byte *msg, byte msgSize);
};

#endif
