#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Courier.h"

Courier::Courier(Stream* stream, void (*callback)(char))
{
  _stream = stream;
  _callback = callback;
  _asciiMode = true;
}

void Courier::update() {
  if (_stream->available())
  {
    _callback(_stream->read());
    
    while (_stream->read() != _cmdDelimiter);
  }
}

bool Courier::hasMoreArgs() {
  _skipArgDelimiters();
  return (_stream->peek() != _cmdDelimiter);
}

char Courier::nextChar()
{
  _skipArgDelimiters();
  return (char)_stream->read();
}

byte Courier::nextByte()
{
  _skipArgDelimiters();
  if (_asciiMode)
    return (byte) _stream->parseInt();
  else 
  {
    byte val;
    _stream->readBytes((char*)&val, sizeof(byte));
    return val;
  }
}

int Courier::nextInt()
{
  _skipArgDelimiters();
  if (_asciiMode)
    return _stream->parseInt();
  else 
  {
    int val;
    _stream->readBytes((char*)&val, sizeof(int));
    return val;
  }
}

float Courier::nextFloat() 
{
  _skipArgDelimiters();
  if (_asciiMode)
    return _stream->parseFloat();
  else 
  {
    float val;
    _stream->readBytes((char*)&val, sizeof(float));
    return val;
  }
}


void Courier::beginSend(char cmd, bool end)
{
  _stream->write(cmd);
  if (end)
    endSend();
}

void Courier::sendChar(char c) 
{
  if (_asciiMode)
    _stream->write(_argDelimiter);
  _stream->write(c);
}

void Courier::sendByte(byte v) 
{
  sendChar((char)v);
}

void Courier::sendInt(int v) 
{
  if (_asciiMode)
  {
    _stream->write(_argDelimiter);
    _stream->print(v);
  }
  else
    _stream->write((const uint8_t*)&v, sizeof(int));
}

void Courier::sendFloat(float v) 
{
  if (_asciiMode) 
  {
    _stream->write(_argDelimiter);
    _stream->print(v);
  }
  else
    _stream->write((const uint8_t*)&v, sizeof(float));
}
  
void Courier::endSend() {
  _stream->write(_cmdDelimiter);
}

void Courier::_skipArgDelimiters(size_t minChars) {
  while (!_stream->available()) {
    while (_stream->peek() == _argDelimiter)
      _stream->read();
  }
}

//void Courier::_readBytes(char* buffer, size_t length) 
//{
//  // TODO: check if number of bytes read == length to catch errors
//  _stream->readBytesUntil(_cmdDelimiter, buffer, length);
//}



