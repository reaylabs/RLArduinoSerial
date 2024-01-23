/*
PrintMenuExample.ino

Description
  This example shows how to print an automated menu to the Serial Monitor. 

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  //Create a RLArduinoSerial object with the termintor = \n

//number of commands in the menu
uint16_t menuSize;

void cmdEnterFloat() {
  Serial.print("Enter A Float Value: "); 
  while(!s.floatAvailable(true)); 
  Serial.println(s.getFloat(),5);
}

void cmdEnterLong() {
  Serial.print("Enter A Long Value: "); 
  while(!s.longAvailable(true)); 
  Serial.println(s.getLong());
}

//command array
SerialCommandInfo commandArray[] = {
  {1, "Float", cmdEnterFloat},
  {2, "Long", cmdEnterLong}
};

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("RLArduinoSerial Version: " + s.version()+ "\n");
  menuSize = sizeof(commandArray) / sizeof(SerialCommandInfo);
  s.printMenu(commandArray, menuSize);
}

void loop() {
  int cmd;
  //Check if a command is available
  if (s.longAvailable(true)) 
  {
    cmd = s.getLong();
    Serial.println(cmd);
    for (int i = 0 ; i < menuSize ; i++) {
      if (commandArray[i].command == cmd) {
        commandArray[i].functionPointer();
        break;
      }
    }
    s.printMenu(commandArray, menuSize);
  }
}