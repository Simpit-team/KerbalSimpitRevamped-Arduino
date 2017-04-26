#include "KerbalSimPit.h"

KerbalSimPit::KerbalSimPit(Stream &serial)
{
  _serial = &serial;
}

bool KerbalSimPit::init()
{
  if (_serial == NULL) {
    return false;
  }
  _outboundBuffer[0] = 0x00;
  _outboundBuffer[1] = 0x37;
  _receiveState = WaitingFirstByte;
  send(0x00, _outboundBuffer, 2); // Send SYN
  while (!_serial->available());
  if (_serial->read() == 0xAA) { // First byte of header
    while (!_serial->available());
    if (_serial->read() == 0x50) { // Second byte of header
      while (!_serial->available());
      _serial->read(); // size
      while (!_serial->available());
      if (_serial->read() == 0x00) { // type
        while (!_serial->available());
        if (_serial->read() == 0x01) { // first byte of payload, we got a SYNACK
          // TODO: Do we care about tracking handshake state?
          _outboundBuffer[0] = 0x02;
          send(0x00, _outboundBuffer, 2); // Send ACK
          return true;
        }
      }
    }
  }
  return false;
}

void KerbalSimPit::inboundHandler(void (*messageHandler)(byte messageType,
                                                        byte msg[],
                                                        byte msgSize))
{
  _messageHandler = messageHandler;
}

void KerbalSimPit::registerChannel(byte channelID)
{
  send(REGISTER_MESSAGE, &channelID, 1);
}

void KerbalSimPit::deregisterChannel(byte channelID)
{
  send(DEREGISTER_MESSAGE, &channelID, 1);
}

void KerbalSimPit::send(byte PacketType, byte msg[], byte msgSize)
{
  _serial->write(0xAA);
  _serial->write(0x50);
  _serial->write(msgSize);
  _serial->write(PacketType);
  for (int x=0; x<msgSize; x++) {
    _serial->write(*(msg+x));
  }
}

void KerbalSimPit::update()
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
      _inboundSize = _readBuffer;
      _receiveState = WaitingType;
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
