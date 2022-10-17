# RLArduinoSerial
The RLArduinoSerial class implements a blocking or non-blocking serial read function.
Whenever the checkForData() function or the stringAvailable(true), longAvailable(true),
floatAvailable(true), doubleAvailable(true) functions are called, all characters in the 
Arduino Serial input buffer will be read into an internal buffer. When the
the termination character is found, an attempt will be automatically made to convert
the buffer data into a long, float, double or String. The appropriate xxxxAvailable() 
function with no parameter can check if the conversion was successful without running
the checkForData function. The value can then be retrieved by calling the appropriate 
getxxxx function which will then reset the available flag until a new data set is received 
and the available flags updated. 


Data Type Examples:

    Input Type    Input Value   Output Type        
    integer       "123"         long,float,double,string
    binary        "0b1011"      long,string
    hex           "0xFF"        long,string
    float         "1.23e-9"     float,double,string
    string        "ABCDEF$%"    string

## Class RLArduinoSerial
```C++
explicit  RLArduinoSerial(char terminator);
void      checkForData();
bool      doubleAvailable(bool runCheckForData = false);
bool      floatAvailable(bool runCheckForData = false);
double    getDouble();
float     getFloat();
long      getLong();
String    getString();
char      getTerminator();
bool      longAvailable(bool runCheckForData = false);
void      setTerminator(char terminator);
bool      stringAvailable(bool runCheckForData = false);
```

## Example
```C++
#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop()
{
  runNonBlockingExample();
}

// Send following string to test this routine:
// 123!-456!0xFF!0b1011!1.23456789012345!-4.5e3!ABC#$%!
void runNonBlockingExample()
{
  s.setTerminator('!');
  s.checkForData();
  if (s.stringAvailable())
  {
    Serial.print("\nString: ");
    Serial.println(s.getString());
  }
  if (s.longAvailable())
  {
    Serial.print("Long: ");
    Serial.println(s.getLong());
  }
  if (s.floatAvailable())
  {
    Serial.print("Float:  ");
    Serial.println(s.getFloat(),15);
  }
  if (s.doubleAvailable())
  {
    Serial.print("Double: ");
    Serial.println(s.getDouble(),15);
  }

  //Do something here while waiting for input
}
```
Output for 123!-456!0xFF!0b1011!1.234567890123456!-4.5e3!ABC#$%!
```
String: 123
Long: 123
Float:  123.000000000000000
Double: 123.000000000000000

String: -456
Long: -456
Float:  -456.000000000000000
Double: -456.000000000000000

String: 0xFF
Long: 255

String: 0b1011
Long: 11

String: 1.234567890123456
Float:  1.234567880630493
Double: 1.234567890123456

String: -4.5e3
Float:  -4500.000000000000000
Double: -4500.000000000000000

String: ABC#$%
```