/*
 * Minimalist serial protocol library for Arduino-Wiring with low memory 
 * trace that allows switching between ASCII and binary modes.
 * 
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Courier.h"

Courier::Courier(void (*callback)(char), Stream* stream)
{
  _stream = stream;
  _callback = callback;
  _asciiMode = true;
}

void Courier::update() {
  if (_stream->available())
  {
    _callback(_stream->read());
    
    while (_stream->read() != COURIER_CMD_DELIMITER);
  }
}

bool Courier::hasMoreArgs() {
  _skipArgDelimiters();
  return (_stream->peek() != COURIER_CMD_DELIMITER);
}

char Courier::nextChar()
{
  while (true) {
    if (_stream->available()) {
      int c = _stream->read();
      if (c != COURIER_ARG_DELIMITER)
        return c;
    }
  }
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

void Courier::_skipArgDelimiters() {
  while (!_stream->available()) {
    while (_stream->peek() == COURIER_ARG_DELIMITER)
      _stream->read();
  }
}

//void Courier::_readBytes(char* buffer, size_t length) 
//{
//  // TODO: check if number of bytes read == length to catch errors
//  _stream->readBytesUntil(COURIER_CMD_DELIMITER, buffer, length);
//}



