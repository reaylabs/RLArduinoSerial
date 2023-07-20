/*
WaitWithTimeout.ino

Description
  This is an example of waiting for a value with a timeout using the RLArduinoSerial class.
  A callback function can be called while waiting for the value.

  Open up the Serial Monitor, set the terminator to New Line and the baud rate to 9600.
  Enter values into the Message box and send to the board. 

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n', 10000);  //Create a RLArduinoSerial object with the termintor = \n and timeout = 10000 ms

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("RLArduinoSerial Version: " + s.version()+ "\n");
}

void loop() {
  String stringValue;
  long longValue;
  //Wait for a String value while running the callback function while waiting
  Serial.println("\nEnter A StringValue With Callback Function : ");
  if (s.waitForStringWithTimeout(&stringValue, callbackFunction)) {
    Serial.println(stringValue);
  } else {
    Serial.println("timeout");
  }
  Serial.println("Press the Enter key to continue");
  while(!s.stringAvailable(true));

  //Wait for a long value with no callback function
  Serial.println("\nEnter A Long Value With No Callback Function : ");
  if (s.waitForLongWithTimeout(&longValue, NULL)) {
    Serial.println(longValue);
  } else {
    Serial.println("timeout");
  }
  Serial.println("Press the Enter key to continue");
  while(!s.stringAvailable(true));
}

void callbackFunction() {
  delay(500);
  Serial.println("Callback");
}