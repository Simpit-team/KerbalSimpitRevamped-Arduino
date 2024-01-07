/** @file KerbalSimpit.cpp
    Core library functions.
*/
#include "KerbalSimpit.h"

KerbalSimpit::KerbalSimpit(Stream &serial)
{
  _serial = &serial;
  packetDroppedNbr = 0;
}

bool KerbalSimpit::init()
{
  if (_serial == NULL) {
    return false;
  }

  // First, empty the receiving buffer to avoid looking into it looking for the SYNACK
  while (_serial->available())
	_serial->read();

  // Format and send a SYN packet
  _outboundBuffer[0] = 0x00;
  unsigned int i;
  for (i=0; i<sizeof(KERBALSIMPIT_VERSION); i++) {
    _outboundBuffer[i+1] = KERBALSIMPIT_VERSION[i];
  }
  i = i + 1;
  _send(0x00, _outboundBuffer, i); // Send SYN

  // Wait for an answer. If no answer in 1 sec, return false
  uint8_t count = 0;
  while (!_serial->available()){
    count += 1;
    delay(100);
    if(count > 10){
      return false;  
    }
  }

  _receivedIndex = 0;
  while(_serial->available()){
    _inboundBuffer[_receivedIndex] = _serial->read();
    if(!_inboundBuffer[_receivedIndex]){
      cobsDecode(_inboundBuffer, _receivedIndex + 1, _inboundDecodedBuffer);
      _receivedIndex = 0;
      
      // Test is the message received is a SYNACK
      if(_inboundDecodedBuffer[0] == SYNC_MESSAGE && _inboundDecodedBuffer[1] == 0x01){
		if (_inboundDecodedBuffer[2] == KERBAL_SPACE_PROGRAM_2) _connectedToKSP2 = true;
        else _connectedToKSP2 = false;
        _outboundBuffer[0] = 0x02;
        _send(0x00, _outboundBuffer, i); // Send ACK

        // Log the serial buffer size
        #ifdef SERIAL_RX_BUFFER_SIZE
          printToKSP("Buffer receive size : " + String(SERIAL_RX_BUFFER_SIZE));
        #else
          printToKSP("Buffer receive size not found");
        #endif
        return true;
      } else {
        return false;
      }
    }
    _receivedIndex ++;
  }

  return false;
}

void KerbalSimpit::inboundHandler(void (*messageHandler)(byte messageType,
                                                        byte msg[],
                                                        byte msgSize))
{
  _messageHandler = messageHandler;
}

void KerbalSimpit::closeSerialConnection()
{
  _send(CLOSE_SERIAL, 0, 1);
}

void KerbalSimpit::registerChannel(byte channelID)
{
  _send(REGISTER_MESSAGE, &channelID, 1);
}

void KerbalSimpit::deregisterChannel(byte channelID)
{
  _send(DEREGISTER_MESSAGE, &channelID, 1);
}

void KerbalSimpit::requestMessageOnChannel(byte channelID){
  _send(REQUEST_MESSAGE, &channelID, 1);
}

void KerbalSimpit::printToKSP(String msg){
	printToKSP(msg, 0);
}

void KerbalSimpit::printToKSP(String msg, byte options){
  byte payload[32];
  payload[0] = options;

  bool hasDataLeft = true;
  for(unsigned int i = 0; i < 31; i++){
    if(msg[i] == '\0'){
      hasDataLeft = false;
    }

    if(!hasDataLeft){
      payload[i+1] = '\0';
    } else {
      payload[i+1] = msg[i];
    }
  }

  _send(CUSTOM_LOG, payload, 32);
}

void KerbalSimpit::_send(byte messageType, byte msg[], byte msgSize)
{
  _msgBuffer[0] = messageType;
  byte checksum = messageType;
  for(byte i = 0; i < msgSize; i++){
    _msgBuffer[i+1] = msg[i];
    checksum ^= msg[i];
  }
  _msgBuffer[msgSize+1] = checksum;

  byte encodedMsgSize = cobsEncode(_msgBuffer, msgSize + 2, _encodedBuffer);
  for (byte x=0; x<encodedMsgSize; x++) {
    _serial->write(_encodedBuffer[x]);
  }
  _serial->write((byte) 0); // Encoded buffer does not have the null terminating byte.
}

void KerbalSimpit::update()
{
  while(_serial->available()){
    _inboundBuffer[_receivedIndex] = _serial->read();
    _receivedIndex ++;
    
    if(!_inboundBuffer[_receivedIndex - 1]){
      byte decodedSize = cobsDecode(_inboundBuffer, _receivedIndex + 1, _inboundDecodedBuffer);
      
      // Account for the overhead of 1 byte of COBS
      if(decodedSize != _receivedIndex - 1){
        // Ill-formed packet
        packetDroppedNbr ++;
      } else {
        // Check checksum
        byte checksum = 0;
        for(byte x = 0; x < decodedSize - 2; x++){
          checksum ^= _inboundDecodedBuffer[x];
        }
        if(checksum != _inboundDecodedBuffer[decodedSize-2]){
          // Discard message, bad checksum
          packetDroppedNbr ++;
        } else {
          _messageHandler(_inboundDecodedBuffer[0], _inboundDecodedBuffer + 1, decodedSize - 3);
        }
      }
      _receivedIndex = 0;
    }
  }
}

size_t KerbalSimpit::cobsEncode(const void *data, size_t length, uint8_t *buffer)
{
  uint8_t *encode = buffer; // Encoded byte pointer
  uint8_t *codep = encode++; // Output code pointer
  uint8_t code = 1; // Code value

  for (const uint8_t *byte = (const uint8_t *)data; length--; ++byte)
  {
    if (*byte) // Byte not zero, write it
      *encode++ = *byte, ++code;

    if (!*byte || code == 0xff) // Input is zero or block completed, restart
    {
      *codep = code, code = 1, codep = encode;
      if (!*byte || length)
        ++encode;
    }
  }
  *codep = code; // Write final code value

  return (size_t)(encode - buffer);
}

size_t KerbalSimpit::cobsDecode(const uint8_t *buffer, size_t length, void *data)
{
  const uint8_t *byte = buffer; // Encoded input byte pointer
  uint8_t *decode = (uint8_t *)data; // Decoded output byte pointer

  for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block)
  {
    if (block) // Decode block byte
      *decode++ = *byte++;
    else
    {
      if (code != 0xff) // Encoded zero, write it
        *decode++ = 0;
      block = code = *byte++; // Next block length
      if (!code) // Delimiter code found
        break;
    }
  }

  return (size_t)(decode - (uint8_t *)data);
}
