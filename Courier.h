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
#ifndef COURIER_H_
#define COURIER_H_

#include <Arduino.h>

#define COURIER_ARG_DELIMITER ' '
#define COURIER_CMD_DELIMITER '\n'

class Courier 
{
private:
  Stream* _stream;
  void (*_callback)(char);
  bool _asciiMode;
  
public:
  Courier(void (*callback)(char), Stream* stream=&Serial);

  bool isAsciiMode() const { return _asciiMode; }
  void setAsciiMode(bool asciiMode) { _asciiMode = asciiMode; }

  void update();
  
  // WARNING: Only works in ascii mode.
  //bool hasMoreArgs();
  
  char nextChar();
  byte nextByte();
  int nextInt();
  float nextFloat();
  
  void beginSend(char cmd);
  void sendChar(char c);
  void sendByte(byte v);
  void sendInt(int v);
  void sendFloat(float v);
  void endSend();

  // Send no-arguments command.
  void sendNoArgs(char cmd);

private:
  void _skipArgDelimiters();
//  void _readBytes(char* buffer, size_t length);  
};


#endif
