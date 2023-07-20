/*
MenuExample.ino

Description
  This implements a nested menu exampled.

Author
  Robert Reay
*/

#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  //Create a RLArduinoSerial object with the termintor = \n

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("RLArduinoSerial Version: " + s.version()+ "\n");
  printMenu();
}

void loop() {
  int command;
  while(!s.longAvailable(true)); //Wait for a command
  command = s.getLong();
  Serial.println(command);
  switch (command) {
    case 1: 
      Serial.print("Enter A Float Value: "); 
      while(!s.floatAvailable(true)); 
      Serial.println(s.getFloat(),5);
    break; 
    case 2:
      Serial.print("Enter A Double Value: "); 
      while(!s.doubleAvailable(true)); 
      Serial.println(s.getDouble());
      break;
    case 3:
      Serial.print("Enter A Long Value: "); 
      while(!s.longAvailable(true)); 
      Serial.println(s.getLong());
      break;
    case 4:
      Serial.print("Enter A String Value: "); 
      while(!s.stringAvailable(true)); 
      Serial.println(s.getString());
      break;
  }
  printMenu();
}

void printMenu()
{
  Serial.println("\n*** Commands ***");
  Serial.println("1: Enter Float Value");
  Serial.println("2: Enter Double Value");
  Serial.println("3: Enter Long Value");
  Serial.println("4: Enter String Value");
  Serial.print("Enter Command: ");
}