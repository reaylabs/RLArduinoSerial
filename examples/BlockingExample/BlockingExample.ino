/*
BlockingExample.ino

Description
  This is an blocking example using the RLArduinoSerial class.
  The program will wait until a valid float value is received.
  The runCheckForData parameter is set to true so
  the Serial input buffer will be continually read.

  Open up the Serial Monitor, set the terminator to New Line and the baud rate to 9600.
  Enter values into the Message box and send to the board. 

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  //Create a RLArduinoSerial object with the termintor = \n

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Serial.print("Enter A Float Value : ");
  while(!s.floatAvailable(true));   //Parse the Serial input buffer and check for a Float
  Serial.println(s.getFloat(),10);
  Serial.println();
}