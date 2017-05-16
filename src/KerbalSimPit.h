#ifndef KerbalSimPit_h
#define KerbalSimPit_h

#include <Arduino.h>

#include "KerbalSimPitMessageTypes.h"
#include "PayloadStructs.h"

const char KERBALSIMPIT_VERSION[] = "0.8"; /**< Library version sent to the plugin for compatibility checking. */
const byte MAX_PAYLOAD_SIZE = 32; /**< Maximum payload size does not include header. */

/** The KerbalSimPit class manages a serial connection to KSP.
    It automates the handshaking process, and provides utility
    functions to encapsulate most message types in and out of
    the game.
 */
class KerbalSimPit
{
 public:
  /** Default constructor.
      @param serial The serial instance this instance will use to communicate
      with the plugin. Usually "Serial".
  */
  KerbalSimPit(Stream &serial);
  /** Initialise the serial connection.
      Performs handshaking with the plugin. Note that the KSPit library
      *does not* call the `begin()` method on the serial object. You'll need
      to ensure you've run `Serial.begin(115200)` (or similar) before calling
      this method.
  */
  bool init();
  /** Specify a callback function to handle messages received from the plugin.
      @see messageHandler
      @param messageHandler The callback function.
  */
  void inboundHandler(void (*messageHandler)(byte messageType,
                                            byte msg[], byte msgSize));
  /** Subscribe to a channel of messages coming from the plugin.
      This function sends a channel subscription message to the plugin,
      indicating that this device would like to receive messages send
      to a given channel.
      @param channelID The ID of the channel to subscribe to.
  */
  void registerChannel(byte channelID);
  /** Unsubscribe from a channel of messages coming from the plugin.
      This function sends a channel subscription message to the plugin,
      indicating that no further messages for the given channel should
      be sent to this device.
      @param channelID The ID of the channel to unsubscribe from.
  */
  void deregisterChannel(byte channelID);
  /** Send a formatted KSPit packet.
      Sends the given message as payload of a KSPit message.
      @param messageType The ID of the message channel.
      @param msg A byte array representing the message contents. Details of
      what this should consist of are specific to the message type.
      @param msgSize The size of msg.
  */
  void send(byte messageType, byte msg[], byte msgSize);
  /** Regular library update function.
      This function polls the serial device for new data, and performs other
      tasks that must be done regularly. The function should be called from
      an Arduino sketch `loop()` method.
  */
  void update();

  // Helper functions
  void activateCAG(byte actiongroup);
  void deactivateCAG(byte actiongroup);
  void toggleCAG(byte actiongroup);
  void activateAction(byte action);
  void deactivateAction(byte action);
  void toggleAction(byte action);

 private:
  byte _readBuffer;
  byte _inboundType;
  byte _inboundSize;
  byte _inboundBuffer[MAX_PAYLOAD_SIZE];
  byte _outboundBuffer[MAX_PAYLOAD_SIZE];
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

  void (*_messageHandler)(byte messageType, byte msg[], byte msgSize);
};

#endif
