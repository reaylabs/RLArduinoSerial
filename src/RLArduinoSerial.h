/*
RLArduinoSerial.h

Description
  Non-blocking serial class 

Author
  Robert Reay

Revision History
  10-14-2022 : Initial Code
*/

#ifndef _RL_ARDUINO_SERIAL_H_
#define _RL_ARDUINO_SERIAL_H_

#include "Arduino.h"

#define inputBufferSize 64

class RLArduinoSerial {
  public:
    explicit  RLArduinoSerial(char terminator);
    void checkForData();
    bool doubleAvailable(bool runCheckForData = false);
    bool  floatAvailable(bool runCheckForData = false);
    double getDouble();
    float getFloat();
    String getString();
    long getLong();
    char getTerminator();
    bool longAvailable(bool runCheckForData = false);
    void setTerminator(char terminator);
    bool stringAvailable(bool runCheckForData = false);
  
  private:
    bool _convertToDouble(char *buffer, double *value, char terminator);
    bool _convertToFloat(char *buffer, float *value, char terminator);
    bool _convertToLong(char *buffer, long *value, char terminator);
    double _doubleValue;
    bool _doubleAvailable;
    bool _floatAvailable;
    float _floatValue;
    bool _foundTerminator;
    char _inputBuffer[inputBufferSize+1];
    int  _inputIndex;
    bool _isBinary();
    bool _isHex();
    bool _longAvailable;
    long _longValue;
    char _terminator;
    void _reset();
    bool _stringAvailable;
    String _stringValue;
};

#endif // _RL_ARDUINO_SERIAL_H_