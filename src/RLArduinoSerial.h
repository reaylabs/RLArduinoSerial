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
#define RL_ARDUINO_SERIAL_LIB_VERSION  (F("1.5.0"))

typedef void (*FunctionPointer)(void);

class RLArduinoSerial {
  public:
    explicit  RLArduinoSerial(char terminator, int timeout = 1000);
    void checkForData();
    bool doubleAvailable(bool runCheckForData = false);
    bool  floatAvailable(bool runCheckForData = false);
    double getDouble();
    float getFloat();
    String getString();
    long getLong();
    char getTerminator();
    bool longAvailable(bool runCheckForData = false);
    void printEngineeringFormat(float value, int totalDigits, String units = "", int minRange = -12);
    void printlnEngineeringFormat(float value, int totalDigits, String units = "", int minRange = -12);
    void setTerminator(char terminator);
    void setTimeout(int timeout);
    bool stringAvailable(bool runCheckForData = false);
    String version();
    bool waitForDoubleWithTimeout(double *value, FunctionPointer callback);
    bool waitForFloatWithTimeout(float *value, FunctionPointer callback);
    bool waitForLongWithTimeout(long *value, FunctionPointer callback);
    bool waitForStringWithTimeout(String *value, FunctionPointer callback);
  
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
    int _timeout;
    void _reset();
    bool _stringAvailable;
    const String _version = F("1.3.0");
    String _stringValue;
};

#endif // _RL_ARDUINO_SERIAL_H_