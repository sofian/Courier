#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Courier.h"

Courier::Courier(void (*callback)(char), Stream* stream)
  : _callback(callback), _stream(stream), _asciiMode(true)
{
}

void Courier::update() {
  if (_stream->available())
  {
    // Call callback function with command.
    _callback(nextChar());
    
    // Flush rest of command (ie. until delimiter is reached).
    while (_stream->read() != COURIER_CMD_DELIMITER);
  }
}

//bool Courier::hasMoreArgs() {
//  _skipArgDelimiters();
//  return (_stream->peek() != COURIER_CMD_DELIMITER);
//}

char Courier::nextChar()
{
  // Find the next char.
  char c;
  while (true)
  {
    if (_stream->available())
    {
      c = _stream->read();
      if (c == COURIER_CMD_DELIMITER)
        return 0; // end-of-command: sudden death => return null character
      else if (c != COURIER_ARG_DELIMITER)
        break;
    }
  }
  // Skip the following non-delimiter chars (ascii mode only).
  if (_asciiMode) {
    char flushChar;
    while (true) {
      if (_stream->available()) {
        flushChar = _stream->peek();
        if (flushChar == COURIER_CMD_DELIMITER || flushChar == COURIER_ARG_DELIMITER)
          break;
        else
          _stream->read(); // eat char
      }
    }
  }
  // Return character.
  return c;
}

uint8_t Courier::nextByte()
{
  if (_asciiMode)
  {
    return (uint8_t) _stream->parseInt();
  }
  else 
  {
    uint8_t val;
    _stream->readBytes((char*)&val, sizeof(uint8_t));
    return val;
  }
}

int16_t Courier::nextInt()
{
  if (_asciiMode)
  {
    return _stream->parseInt();
  }
  else 
  {
    int val;
    _stream->readBytes((char*)&val, sizeof(int16_t));
    return val;
  }
}

float Courier::nextFloat()
{
  if (_asciiMode)
  {
    return _stream->parseFloat();
  }
  else 
  {
    float val;
    _stream->readBytes((char*)&val, sizeof(float));
    return val;
  }
}


void Courier::beginSend(char cmd)
{
  _stream->write(cmd);
}

void Courier::sendChar(char c) 
{
  if (_asciiMode)
    _stream->write(COURIER_ARG_DELIMITER);
  _stream->write(c);
}

void Courier::sendByte(byte v) 
{
  if (_asciiMode)
  {
    _stream->write(COURIER_ARG_DELIMITER);
    _stream->print(v);
  }
  else
    _stream->write(v);
}

void Courier::sendInt(int v) 
{
  if (_asciiMode)
  {
    _stream->write(COURIER_ARG_DELIMITER);
    _stream->print(v);
  }
  else
    _stream->write((const uint8_t*)&v, sizeof(int));
}

void Courier::sendFloat(float v) 
{
  if (_asciiMode) 
  {
    _stream->write(COURIER_ARG_DELIMITER);
    _stream->print(v);
  }
  else
    _stream->write((const uint8_t*)&v, sizeof(float));
}
  
void Courier::endSend() {
  _stream->write(COURIER_CMD_DELIMITER);
}

void Courier::sendNoArgs(char cmd) {
  beginSend(cmd);
  endSend();
}

//void Courier::_skipArgDelimiters() {
//  while (!_stream->available()) {
//    while (_stream->peek() == COURIER_ARG_DELIMITER)
//      _stream->read();
//  }
//}

//void Courier::_readBytes(char* buffer, size_t length) 
//{
//  // TODO: check if number of bytes read == length to catch errors
//  _stream->readBytesUntil(COURIER_CMD_DELIMITER, buffer, length);
//}



