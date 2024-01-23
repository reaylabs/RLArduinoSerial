/*
RLArduinoSerial.cpp

Description
  Arduino serial communition helper class

Author
  Robert Reay

Revision History
  10-14-2022 : Initial Code
*/

#include "RLArduinoSerial.h"

//Constructor
RLArduinoSerial::RLArduinoSerial(char terminator, int timeout)
{
  _terminator = terminator;
  _timeout = timeout;
  _reset();
}

bool RLArduinoSerial::_convertToDouble(char *buffer, double *value, char terminator)
{
  bool convertOK = false;
  char *pEnd;
  if (!_isBinary() && !_isHex())
  {
    *value = strtod (buffer,&pEnd);
    convertOK = (pEnd[0] == terminator);
  }
  return convertOK;
}

bool RLArduinoSerial::_convertToFloat(char *buffer, float *value, char terminator)
{
  bool convertOK;
  char *pEnd;
  if (!_isBinary() && !_isHex())
  {
    *value = strtof (buffer,&pEnd);
    convertOK = (pEnd[0] == terminator);
  }  else convertOK = false;
  return convertOK;
}

bool RLArduinoSerial::_convertToLong(char *buffer, long *value, char terminator) 
{
  bool convertOK;
  char *pEnd;
  //Base 10
  *value = strtol (buffer,&pEnd,10);
  convertOK = (pEnd[0] == terminator);
  //Base 16 (hex)
  if (!convertOK)
  {
    *value = strtol (buffer,&pEnd,0);
    convertOK = (pEnd[0] == terminator);  
  }
  //Base 2 (binary)
  if (!convertOK)
  {
    if (_isBinary())
    {
      buffer[0] = ' '; buffer[1] = ' ';
    }
    *value = strtol (buffer,&pEnd,2);
    convertOK = (pEnd[0] == terminator);  
  }
  return convertOK;
}

bool RLArduinoSerial::doubleAvailable(bool runCheckForData)
{
  if (runCheckForData)
  {
    _reset();
    checkForData();
  }
  return _doubleAvailable;
}

bool RLArduinoSerial::floatAvailable(bool runCheckForData)
{
  if (runCheckForData)
  {
    _reset();
    checkForData();
  }
  return _floatAvailable;
}

bool RLArduinoSerial::_isBinary()
{
  if (_stringValue[0] == '0' && _stringValue[1] == 'b')
  {

    return true;
  } else return false;
}

bool RLArduinoSerial::_isHex()
{
  if ((_stringValue[0] == '0' && _stringValue[1] == 'x') || (_stringValue[0] == '0' && _stringValue[1] == 'X'))
  {
    return true;
  } else return false;
}

double RLArduinoSerial::getDouble()
{
  double d = _doubleValue;
  _doubleValue = 0;
  _doubleAvailable = false;
  return d;
}

float RLArduinoSerial::getFloat()
{
  float f = _floatValue;
  _floatValue = 0;
  _floatAvailable = false;
  return f;
}

String RLArduinoSerial::getString()
{
  _stringAvailable  = false;
  return _stringValue;
}

long RLArduinoSerial::getLong()
{
  _longAvailable = false;
  return _longValue;;
}

char RLArduinoSerial::getTerminator()
{
  return _terminator;
}

bool RLArduinoSerial::longAvailable(bool runCheckForData)
{
  if (runCheckForData) 
  {
    _reset();
    checkForData();
  }
  return _longAvailable;
}

void RLArduinoSerial::checkForData()
{
  _foundTerminator = false;

  //Parse the Serial input buffer
  while ((Serial.available() > 0) && !_foundTerminator)
  {
    char c = (char)Serial.read(); 

    //Add the next Serial char to the input buffer if not lf or cr         
    if( (c != '\n' && c != '\r') || c == _terminator)             
    {
      //prevent buffer overflow
      if (_inputIndex >= inputBufferSize)   
      {
        _inputIndex = 0;
      }
      //Update the string value
       _stringValue = _inputBuffer;

      //add the char to the input buffer and set the termination character
      _inputBuffer[_inputIndex] = c;        
      _inputBuffer[_inputIndex + 1] = '\0'; 
      _inputIndex++;
    }

    //Look for the terminator character
    if (c == _terminator)
    {
      _foundTerminator = true;

      //Try conversion to long
      long newLong;
      if (_convertToLong(_inputBuffer, &newLong, _terminator))
      {
        _longAvailable = true;
        _longValue = newLong;
      }

      //Try conversion to float
      float newFloat;
      if (_convertToFloat(_inputBuffer, &newFloat, _terminator))
      {
        _floatAvailable = true;
        _floatValue = newFloat;
      }

      //Try conversion to double
      double newDouble;
      if (_convertToDouble(_inputBuffer, &newDouble, _terminator))
      {
        _doubleAvailable = true;
        _doubleValue = newDouble;
      }

      //String is now available
      _stringAvailable = true;

      //Clear the input buffer since the termination character was found
      _inputIndex = 0;
      _inputBuffer[0] = '\0';
    }
  }
}

void RLArduinoSerial::printMenu(SerialCommandInfo *menu, uint16_t menuSize)
{
  Serial.println("\n*** Commands ***");
  for (int i = 0; i < menuSize; i++)
  {
    Serial.print(menu[i].command);
    Serial.print(": ");
    Serial.println(menu[i].title);
  }
  Serial.print("Enter Command: ");
}

void RLArduinoSerial::_reset()
{
    _foundTerminator = false;
    _doubleAvailable = false;
    _floatAvailable = false;
    _longAvailable = false;
    _stringAvailable = false;
    _longValue = 0;
    _doubleValue = 0;
    _longValue = 0;
    _inputIndex = 0;
    _inputBuffer[0] = '\0';
}

void RLArduinoSerial::setTerminator(char terminator)
{
  _terminator = terminator;
}

void RLArduinoSerial::setTimeout(int timeout)
{
  _timeout = timeout; // in milliseconds
}

bool RLArduinoSerial::stringAvailable(bool runCheckForData)
{
  if (runCheckForData)
  {
    _reset();
    checkForData();
  }
  return _stringAvailable;
}

String RLArduinoSerial::version()
{
  return _version;
}

bool RLArduinoSerial::waitForDoubleWithTimeout(double *value, FunctionPointer callback)
{
  bool success = false;
  long startTime = millis();
  while (!success && (millis() - startTime) < _timeout)
  {
    if (doubleAvailable(true))
    {
      *value = getDouble();
      success = true;
    } else {
      if (callback != NULL)
      {
        callback();
      }
    }
  }
  return success;
}

bool RLArduinoSerial::waitForFloatWithTimeout(float *value, FunctionPointer callback)
{
  bool success = false;
  long startTime = millis();
  while (!success && (millis() - startTime) < _timeout)
  {
    if (floatAvailable(true))
    {
      *value = getFloat();
      success = true;
    } else {
      if (callback != NULL)
      {
        callback();
      }
    }
  }
  return success;
}

bool RLArduinoSerial::waitForLongWithTimeout(long *value, FunctionPointer callback)
{
  bool success = false;
  long startTime = millis();
  while (!success && (millis() - startTime) < _timeout)
  {
    if (longAvailable(true))
    {
      *value = getLong();
      success = true;
    } else {
      if (callback != NULL)
      {
        callback();
      }
    }
  }
  return success;
}

bool RLArduinoSerial::waitForStringWithTimeout(String *value, FunctionPointer callback)
{
  bool success = false;
  long startTime = millis();
  while (!success && (millis() - startTime) < _timeout)
  {
    if (stringAvailable(true))
    {
      *value = getString();
      success = true;
    } else {
      if (callback != NULL)
      {
        callback();
      }
    }
  }
  return success;
}
