#ifndef COURIER_H_
#define COURIER_H_

#include <Arduino.h>

class Courier 
{
private:
  Stream* _stream;
  char _argDelimiter = ' ';
  char _cmdDelimiter = '\n';
  void (*_callback)(char);
  bool _asciiMode;
  
public:
  Courier(Stream* stream, void (*callback)(char));

  void update();
  
  bool isAsciiMode() const { return _asciiMode; }
  void setAsciiMode(bool asciiMode) { _asciiMode = asciiMode; }
  
  bool hasMoreArgs();
  
  char nextChar();
  byte nextByte();
  int nextInt();
  float nextFloat();
  
  void beginSend(char cmd, bool end=false);
  void sendChar(char c);
  void sendByte(byte v);
  void sendInt(int v);
  void sendFloat(float v);
  void endSend();

private:
  void _skipArgDelimiters();
//  void _readBytes(char* buffer, size_t length);  
};


#endif
