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



void RLArduinoSerial::printEngineeringFormat(float value, int totalDigits, String units, int minRange)
{
 float multiplier = pow(10,totalDigits + 1);
 int range = 0;
 if (abs(value) >= 1.0) {
    // Array of suffixes for engineering notation
    char psuffixes[] = {' ', 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};
    
    // Find the appropriate suffix and adjust the value accordingly
    int index = 0;
    //value = round(value * multiplier) / multiplier;
    while (abs(value) >= 1000.0 && index < sizeof(psuffixes) - 1) {
      value /= 1000.0;
      index++;
      range += 3;
    }

    // Calculate the number of digits before the decimal point
    
    int digitsBeforeDecimal;
    if (abs(value) >= 100.0) digitsBeforeDecimal = 3;
    else if (abs(value) >= 10.0) digitsBeforeDecimal = 2;
    else digitsBeforeDecimal = 1;
    
    
    // Calculate the number of digits after the decimal point
    int digitsAfterDecimal = totalDigits - digitsBeforeDecimal;
   if (range  -  minRange < digitsAfterDecimal) {
      digitsAfterDecimal = range - minRange;
    }
    if (digitsAfterDecimal < 0) digitsAfterDecimal = 0;

     if (range - minRange < 0) {
      value = 0,0;
    }
  
    //look for zero
    bool isZero = false;
    if (value == 0.0) isZero = true;
    
    // Print the value with the specified number of digits and the appropriate suffix
    Serial.print(value, digitsAfterDecimal);
    if(psuffixes[index] != ' ') {
      Serial.print(psuffixes[index]);
    }
    Serial.print(units);
  } else {
    
   // Array of suffixes for engineering notation
    char nsuffixes[] = {' ', 'm', 'u', 'n', 'p', 'f', 'a', 'z', 'y'};
    
    // Find the appropriate suffix and adjust the value accordingly
    int index = 0;
    //value = round(value * multiplier) / multiplier;
    while (abs(value) < 1.0 && index < sizeof(nsuffixes) - 1) {
      value *= 1000.0;
      index++;
      range -= 3;
    }

    int digitsBeforeDecimal;
    if (abs(value) >= 100.0) digitsBeforeDecimal = 3;
    else if (abs(value) >= 10.0) digitsBeforeDecimal = 2;
    else digitsBeforeDecimal = 1;
    
    
    // Calculate the number of digits after the decimal point
    int digitsAfterDecimal = totalDigits - digitsBeforeDecimal;

    if (range  -  minRange < digitsAfterDecimal) {
      digitsAfterDecimal = range - minRange;
    }

    if (digitsAfterDecimal < 0) digitsAfterDecimal = 0;

    if (range - minRange < 0) {
      value = 0,0;
    }
  
    //look for zero
    bool isZero = false;
    if (value == 0.0) isZero = true;
    
    // Print the value with the specified number of digits and the appropriate suffix
    Serial.print(value, digitsAfterDecimal);
    if(nsuffixes[index] != ' ' && !isZero) {
      Serial.print(nsuffixes[index]);
    }
    Serial.print(units);
  }
}


void RLArduinoSerial::printlnEngineeringFormat(float value, int totalDigits, String units, int minRange) {
  printEngineeringFormat(value,totalDigits,units, minRange);
  Serial.println();
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
