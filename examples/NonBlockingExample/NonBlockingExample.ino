/*
NonBlockingExample.ino

Description
  This is an simple non-blocking example of the RLArduinoSerial class.
  Open up the Serial Monitor, set the terminator to New Line and the baud rate to 9600.
  Enter values into the Message box and send to the board. 

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

//Global variables
RLArduinoSerial s('\n');  //Create a RLArduinoSerial object with the termintor = \n
long loopCount = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  loopCount = 0;
}

void loop() {
  s.checkForData();         //Parse the Serial input buffer
  if (s.floatAvailable())   //Check for a valid floating point input
  {
    Serial.print("Float Value : ");
    Serial.println(s.getFloat(),10);
    Serial.println((String)"Loop Count: " + loopCount);
    Serial.println();
    loopCount = 0;
  }

  //Do something else while waiting for a float value
  delay(1);
  loopCount++;
}