#include <Arduino.h>
#include <string.h>
// #include <EEPROM.h>

/********** Macros & Varaibles ******************/
#define DEBUG 1
bool stringComplete = false;
String msg = "";

/********* Function Definitions ****************/
void AT_init()
{
#ifdef DEBUG
  Serial.println("...AT Auto Baud rate set...");
#endif
  delay(500);
  Serial2.println("AT");
  delay(100);
  Serial2.println("AT");
}

void serialEvent()
{
  while (Serial2.available())
  {
    // get the new byte:
    char inChar = (char)Serial2.read();
    // add it to the inputString:
    msg += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
}

/************ Setup ****************/
void setup()
{
  Serial2.begin(115200);
  Serial.begin(115200);
  AT_init();
}

/************ Loop ****************/
void loop()
{
  serialEvent();

  /*********To Send AT Commands through Serial monitor**********/
#ifdef DEBUG
  if (Serial.available())
  {                               // If anything comes in Serial (USB),
    Serial2.write(Serial.read()); // read it and send it out Serial2
  }
#endif

  if (stringComplete)
  {
#ifdef DEBUG
    Serial.println(msg);
#endif
    msg = "";
    stringComplete = false;
  }
}