# RLArduinoSerial
  The RLArduinoSerial class implements a blocking or non-blocking serial read function.
Whenever the checkForData() function is called, the characters in the Arduino 
Serial input buffer will be read into an internal buffer until no more characaters are 
available or the termination character is found. When the termination character 
is found, an attempt will be made to convert the buffer data into a long, float, double
or String variable . The appropriate xxxxAvailable() function can then be called to check
if the relavent conversion was successful. The variable value can then be retrieved by 
calling the appropriate getxxxx() function which will then reset the available 
flag until a new data set is received and the available flags updated. 

  The class also implements wait for data functions with a time out and ability
  to run a callback function while waiting for data.

Data Type Examples:

| **Input Type** 	| **Input Value** 	| **Output Type**             	|
|----------------	|-----------------	|-----------------------------	|
| integer        	| "123"           	| long, float, double, String 	|
| binary         	| "0b1011"        	| long, String                	|
| hex            	| "0xFF"          	| long, String                	|
| float/double   	| "1.23e-9"       	| float, double, String       	|
| string         	| "ABCDEF$%"      	| String                      	|

## Class RLArduinoSerial
```C++
explicit  RLArduinoSerial(char terminator);
void      checkForData();
bool      doubleAvailable(bool runCheckForData = false);
bool      floatAvailable(bool runCheckForData = false);
double    getDouble();
float     getFloat();
long      getLong();
String    getString();
char      getTerminator();
bool      longAvailable(bool runCheckForData = false);
void      setTerminator(char terminator);
void      setTimeout(int timeout); //milli seconds
bool      stringAvailable(bool runCheckForData = false);
bool      waitForDoubleWithTimeout(double *value, FunctionPointer callback);
bool      waitForFloatWithTimeout(float *value, FunctionPointer callback);
bool      waitForLongWithTimeout(long *value, FunctionPointer callback);
bool      waitForStringWithTimeout(String *value, FunctionPointer callback);
```

## Non-Blocking Example
```C++
#include "RLArduinoSerial.h"

//Global variables
RLArduinoSerial s('\n');  //Create a RLArduinoSerial object
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
```
## Blocking Example
```C++
#include "RLArduinoSerial.h"

RLArduinoSerial s('\n');  //Create a RLArduinoSerial object 

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  Serial.print("Enter A Float Value : ");
  //Parse the Serial input buffer and check for a Float
  while(!s.floatAvailable(true));   
  Serial.println(s.getFloat(),10);
  Serial.println();
}
```
## Wait For Long Value With Callback Example
```C++
#include "RLArduinoSerial.h"

RLArduinoSerial s('\n', 10000);  //Create a RLArduinoSerial object with timeout and callback

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop() {
  //Wait for a String value while running the callback function while waiting
  String stringValue;
  Serial.println("\nEnter A StringValue With Callback Function : ");
  if (s.waitForStringWithTimeout(&stringValue, callbackFunction)) {
    Serial.println(stringValue);
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
```