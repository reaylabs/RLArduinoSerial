/*
NonBlockingSpeedTest.ino

Description
  This is of parsing function speed.
  Open up the Serial Monitor, set the terminator to New Line and the baud rate to 9600.
  Enter values into the Message box and send to the board. 

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');   //Create a RLArduinoSerial object with the termintor = \n

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("RLArduinoSerial Version: " + s.version()+ "\n");
}

void loop() {
  long start = micros();
  bool stringAvailable = s.stringAvailable(true);
  long end = micros();
  if (stringAvailable)
  {
    Serial.print("String Found: ");
    Serial.println(s.getString()); 
    stringAvailable = false;  
  }
  Serial.println((String)"Process Time (us): " + (end - start));
  Serial.println();
  delay(2000);
}