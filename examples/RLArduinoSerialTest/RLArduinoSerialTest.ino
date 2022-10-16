/*
RLArduinoSerialTest.ino

Description
  Test of the RLArduinoSerial class.

  The RLArduinoSerial class implements a non-blocking serial read function.
  Whenever one of the Available functions is called, all characters in the 
  Arduino Serial input buffer will be read into an internal buffer. When
  the termination character is found, an attempt will be made to convert
  the buffer data into a long, float, double or String. If the conversion is 
  successful, the appropriate available flag will be set. The value
  can then be retrieved using the get function will then reset the
  available flag until a new data set is received and the available flags
  updated.

  Input Examples :
   input type    input string         output types
   ----------   -------------  -------------------------
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
  //speedTest();
  parseTest();
}

void parseTest()
{
  //Test each of the parsing routines
  if (s.longAvailable())
  {
    long l = s.getLong();
    Serial.println((String)"Long Found: " + l);
  }
  if (s.floatAvailable())
  {
    float f = s.getFloat();
    Serial.print("Float Found: ");
    Serial.println(f,12);
  }
  if (s.doubleAvailable())
  {
    double d = s.getDouble();
    Serial.print("Double Found: ");
    Serial.println(d,12);
  }
  if (s.stringAvailable())
  {
    String st = s.getString();
    Serial.print("String Found: ");
    Serial.println(st);
  }

  //Do something else here while wating for input
}

void speedTest()
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
