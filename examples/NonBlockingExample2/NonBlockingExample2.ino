/*
NonBlockingExample2.ino

Description
  This is a more complex non-blocking example of the RLArduinoSerial class that
  will look for all supported data type in the input string.
  Open up the Serial Monitor, set the terminator to New Line and the baud rate to 9600.
  Copy and paste the test string below into the Message box and send to the board.
  For other inputs make sure they include an ! terminator at the end.

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  //Create a RLArduinoSerial object with the termintor = \n

void setup() {
  Serial.begin(9600);
  while(!Serial);
  s.setTerminator('!');
}

// Send following string to test this routine:
// 123!-456!0xFF!0b1011!1.234567890123456!-4.5e3!ABC#$%!
void loop() {

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

  //Do something else while waiting for a float value
}