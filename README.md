# RLArduinoSerial
The RLArduinoSerial class implements a non-blocking serial read function.
Whenever one of the Available functions is called, all characters in the 
Arduino Serial input buffer will be read into an internal buffer. When
the termination character is found, an attempt will be made to convert
the buffer data into a long, float, double or String. If the conversion is 
successful, the appropriate available flag will be set. The value
can then be retrieved using the get function and will then reset the
available flag until a new data set is received and the available flags
updated.

 Data Type Examples:

    Input Type    Input Value        Output Type        
      integer       "123"         long,float,double,string
      binary        "0b1011"      long,string
      hex           "0xFF"        long,string
      float         "1.23e-9"     float,double,string
      string        "ABCDEF$%"    string

## Class RLArduinoSerial
```C++
explicit  RLArduinoSerial(char terminator);
bool      doubleAvailable();
bool      floatAvailable();
double    getDouble();
float     getFloat();
long      getLong();
String    getString();
char      getTerminator();
bool      longAvailable();
void      setTerminator(char terminator);
bool      stringAvailable();
```

## Example
```C++
//Create the RLArduinoSerial Object
RLArduinoSerial s('\n');  

void setup() {
  Serial.begin(9600);
  while(!Serial);
}

void loop()
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
```