/*
printEngineeringString.ino

Description
  This is an example of printing float values as an engineering string with a given number of digits

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
  float value;
  long minRange;
  long totalDigits = 5;
  String units = "V";
  if(s.floatAvailable(true)) {
    value = s.getFloat();
    Serial.println(value,12);
    Serial.print("Enter Total Digits: ");
    s.waitForLongWithTimeout(&totalDigits,NULL);
    Serial.println(totalDigits);
    Serial.print("Enter Min Range: ");
    s.waitForLongWithTimeout(&minRange,NULL);
    Serial.println(minRange);
    s.printlnEngineeringFormat(value, totalDigits, units, minRange);
  }
}

