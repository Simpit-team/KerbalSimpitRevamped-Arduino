/** @file KerbalSimpit.h
    The KerbalSimpit library.
*/
#ifndef KerbalSimpit_h
#define KerbalSimpit_h

#include <Arduino.h>

#include "KerbalSimpitMessageTypes.h"
#include "PayloadStructs.h"

const char KERBALSIMPIT_VERSION[] = "2.4.0"; /**< Library version sent to the plugin for compatibility checking. */
const byte MAX_PAYLOAD_SIZE = 32; /**< Maximum payload size does not include header. Header is 4 chars. */

/** The KerbalSimpit class manages a serial connection to KSP.
    It automates the handshaking process, and provides utility
    functions to encapsulate most message types in and out of
    the game.
 */
class KerbalSimpit
{
 public:
  /** Default constructor.
      @param serial The serial instance this instance will use to communicate
      with the plugin. Usually "Serial".
  */
  KerbalSimpit(Stream &serial);
  /** Initialise the serial connection.
      Performs handshaking with the plugin. Note that the KSPit library
      *does not* call the `begin()` method on the serial object. You'll need
      to ensure you've run `Serial.begin(115200)` (or similar) before calling
      this method.
  */
  bool init();
  /** See which KSP version the controller is connected to.
      Can be used to adjust the controllers behaviour depending on which KSP game is played 
      as not all features are available in both games.
      @return true if connected to KSP2; false if connected to KSP1 or not connected yet.
  */
  bool connectedToKSP2() { return _connectedToKSP2; }
  /** Specify a callback function to handle messages received from the plugin.
      @see messageHandler
      @param messageHandler The callback function.
  */
  void inboundHandler(void (*messageHandler)(byte messageType,
                                            byte msg[], byte msgSize));

  /** Call to request the computer closes the serial connection to this controller.
   *  The computer will deregister all channels that this controller is subscribed to before
   *  closing the connection.
   */
  void closeSerialConnection();

  /** Subscribe to a channel of messages coming from the plugin.
      This function sends a channel subscription message to the plugin,
      indicating that this device would like to receive messages send
      to a given channel.
	  This function should only be called with an ID from the
	  OutboundPackets enum. The IDs from the InboundPackets enum are only
	  used when sending data to KSP and should not be registered.
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
  // This template based on Nick Gammon's I2C_Anything lib:
  // http://www.gammon.com.au/forum/?id=10896&reply=8#reply8
  /** Send a formatted KSPit packet.
      Sends the given message as payload of a KSPit message.
      @param messageType The ID of the message channel.
      @param msg Any object to be sent. The expected object depends on
      the message type. No type checking is done by this library.
  */
  template <typename T> void send(byte messageType, T& msg) {
    _send(messageType, (byte *) &msg, sizeof(msg));
  }
  /** Send a formatted KSPit packet.
      Sends the given message as payload of a KSPit message.
      @param messageType The ID of the message channel.
      @param msg A byte array representing the message contents.
      @param msgSize The size of msg.
  */
  template <typename T> void send(byte messageType, T& msg, byte msgSize) {
    _send(messageType, (byte *) &msg, msgSize);
  }

  /** Regular library update function.
      This function polls the serial device for new data, and performs other
      tasks that must be done regularly. The function should be called from
      an Arduino sketch `loop()` method.
  */
  void update();

  // Helper functions
  /** Activate Custom Action Group.
      Sends a message to activate the given Custom Action Group.
      @param actiongroup The ID of the Custom Action Group to activate.
  */
  void activateCAG(byte actiongroup);
  /** Deactivate Custom Action Group.
      Sends a message to deactivate the given Custom Action Group.
      @param actiongroup The ID of the Custom Action Group to deactivate.
  */
  void deactivateCAG(byte actiongroup);
  /** Toggle Custom Action Group.
      Sends a message to toggle the state of the given Custom Action Group.
      @param actiongroup The ID of the Custom Action Group to toggle.
  */
  void toggleCAG(byte actiongroup);
  /** Activate Action Group.
      Sends a message to activate the given standard Action Group(s).
      @param action A bitfield representing one or more Action Groups
      to activate.
  */
  void activateAction(byte action);
  /** Deactivate Action Group.
      Sends a message to deactivate the given standard Action Group(s).
      @param action A bitfield representing one or more Action Groups
      to deactivate.
  */
  void deactivateAction(byte action);
  /** Toggle Action Group.
      Sends a message to toggle the state of the given standard Action Group(s).
      @param action A bitfield representing one or more Action Groups
      to toggle.
  */
  void toggleAction(byte action);
  /** Set SAS mode
      Send a message to set the desired Autopilot (SAS) mode.
      @param mode The mode to set. Possible modes are listed in the
      AutopilotMode enum.
  */
  void setSASMode(byte mode);

  void setCameraMode(byte mode);

  void cycleNavBallMode();

  void printToKSP(String msg);
  void printToKSP(String msg, byte options);

  /**
   * Ask Simpit to resend a message on the given channel (useful for channels that only send on change).
   * When subscribing on a channel, a message is automatically sent so there is no need to call this during init.
   * May not work on all channels.
   * @param channelID The channel ID to request from (taken from the OutboundPackets enum). Use 0 as a special value for all channels
   */
  void requestMessageOnChannel(byte channelID);

  /**
   * Number of message dropped due to corrupted packet (missing data or bad checksum).
   * This may not be accurate in all cases, some packets can be lost without being noticed here in case of extreme congestion.
   */
  unsigned int packetDroppedNbr;

 private:
  byte _inboundBuffer[MAX_PAYLOAD_SIZE + 4]; // used to store incoming data
  byte _inboundDecodedBuffer[MAX_PAYLOAD_SIZE + 4]; // used to store decoded incoming data.
  byte _outboundBuffer[MAX_PAYLOAD_SIZE]; // used to store the outbound message for initital handshake
  byte _msgBuffer[MAX_PAYLOAD_SIZE + 2]; // used to build the message before sending it (adding type, checksum)
  byte _encodedBuffer[MAX_PAYLOAD_SIZE + 4]; // used to encode message before sending it
  Stream *_serial;
  bool _connectedToKSP2 = false; // used to store wether the controller is connected to KSP1 or KSP2s

  byte _receivedIndex;

  /** Callback function to handle messages from the plugin.
      This function is called by the library when full, correct
      messages are received from the plugin.
      @param messageType The ID of the message channel.
      @param msg A byte array representing the message contents. Note that
      the library does not attempt to transform this byte stream at all,
      and it is up to the user to convert it to the message-specific
      format.
      @param msgSize The size of the msg array.
  */
  void (*_messageHandler)(byte messageType, byte msg[], byte msgSize);

  /** Backend send function.
      @param messageType The ID of the message channel.
      @param msg A byte array representing the message contents.
      @param msgSize The size of msg.
  */
  void _send(byte messageType, byte msg[], byte msgSize);

  /** COBS decode data from buffer. Taken from Wikipedia : https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
    @param buffer Pointer to encoded input bytes
    @param length Number of bytes to decode
    @param data Pointer to decoded output data
    @return Number of bytes successfully decoded
    @note Stops decoding if delimiter byte is found
  */
  size_t cobsDecode(const uint8_t *buffer, size_t length, void *data);

  /** COBS encode data to buffer. Taken from Wikipedia : https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
    @param data Pointer to input data to encode
    @param length Number of bytes to encode
    @param buffer Pointer to encoded output buffer
    @return Encoded buffer length in bytes
    @note Does not output delimiter byte
  */
  size_t cobsEncode(const void *data, size_t length, uint8_t *buffer);
};


#endif
