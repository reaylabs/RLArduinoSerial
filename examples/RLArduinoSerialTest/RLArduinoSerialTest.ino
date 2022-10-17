/*
RLArduinoSerialTest.ino

Description
  Test of the RLArduinoSerial class.

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

    Input Type    Input Value        Output Type        
      integer       "123"         long,float,double,string
      binary        "0b1011"      long,string
      hex           "0xFF"        long,string
      float         "1.23e-9"     float,double,string
      string        "ABCDEF$%"    string

Author
  Robert Reay

Revision History
  10-04-2022 : Initial Code
*/

#include "RLArduinoSerial.h"

//Create the RLArduinoSerial Object
RLArduinoSerial s('\n');  

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  runNonBlockingExample();
  //runBlockingExample();
  //runSpeedTest();
}

// Send following string to test this routine:
// 123!-456!0xFF!0b1011!1.234567890123456!-4.5e3!ABC#$%!
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

void runBlockingExample()
{
  s.setTerminator('\n');
  Serial.print("Enter A Float Value : ");
  while(!s.floatAvailable(true));     //blocking
  Serial.println(s.getFloat(),10);
  Serial.println();
}

void runSpeedTest()
{
  //Determine the speed of the parsing routine
  long start = micros();
  bool stringAvailable = s.stringAvailable();
  long end = micros();
  Serial.println((String)"Process Time (us): " + (end - start));
  if (stringAvailable)
  {
    Serial.print("String Found: ");
    Serial.println(s.getString()); 
    stringAvailable = false;  
  }
  delay(2000);
}
