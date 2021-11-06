/** @file KerbalSimpit.cpp
    Core library functions.
*/
#include "KerbalSimpit.h"

KerbalSimpit::KerbalSimpit(Stream &serial)
{
  _serial = &serial;
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
  _receiveState = WaitingFirstByte;
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

  // Read all the bytes available and look for the SYNACK
  while(_serial->available()){
    if (_serial->read() == 0xAA) { // First byte of header
	  while (!_serial->available()); // Wait for a next byte
	  if (_serial->read() == 0x50) { // Second byte of header
	    while (!_serial->available()); // Wait for a next byte
	    _serial->read(); // size
	    while (!_serial->available()); // Wait for a next byte
	    if (_serial->read() == 0x00) { // type
		  while (!_serial->available()); // Wait for a next byte
		  if (_serial->read() == 0x01) { // first byte of payload, we got a SYNACK
		    // TODO: Do we care about tracking handshake state?
		    _outboundBuffer[0] = 0x02;
		    _send(0x00, _outboundBuffer, i); // Send ACK
		    return true;
		  }
	    }
	  }
    }
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
  _serial->write(0xAA);
  _serial->write(0x50);
  _serial->write(msgSize);
  _serial->write(messageType);
  for (int x=0; x<msgSize; x++) {
    _serial->write(*(msg+x));
  }
}

void KerbalSimpit::update()
{
  while (_serial->available()) {
    _readBuffer = _serial->read();
    switch (_receiveState) {
    case WaitingFirstByte:
      if (_readBuffer == 0xAA) {
        _receiveState = WaitingSecondByte;
      } else {
        _receiveState = WaitingFirstByte;
      }
      break;
    case WaitingSecondByte:
      if (_readBuffer == 0x50) {
        _receiveState = WaitingSize;
      } else {
        _receiveState = WaitingFirstByte;
      }
      break;
    case WaitingSize:
      if (_readBuffer > MAX_PAYLOAD_SIZE) {
	_receiveState = WaitingFirstByte;
      } else {
	_inboundSize = _readBuffer;
	_receiveState = WaitingType;
      }
      break;
    case WaitingType:
      _inboundType = _readBuffer;
      _receivedIndex = 0;
      _receiveState = WaitingData;
      break;
    case WaitingData:
       _inboundBuffer[_receivedIndex] = _readBuffer;
       _receivedIndex++;
       if (_receivedIndex == _inboundSize) {
         _receiveState = WaitingFirstByte;
	 if (_messageHandler != NULL) {
          _messageHandler(_inboundType, _inboundBuffer, _inboundSize);
        }
       }
       break;
    default:
      _receiveState = WaitingFirstByte;
    }
  }
}
