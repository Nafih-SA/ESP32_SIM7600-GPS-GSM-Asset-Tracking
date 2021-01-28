#include <Arduino.h>
#include <string.h>
// #include <EEPROM.h>

/********** Macros & Varaibles ******************/
#define DEBUG 1
bool stringComplete = false;
String msg = "";
struct coords
{
  float Latitude = 0;
  double Longitude = 0;
  float speed = 0;
  char *Hem;
  char E_W;
} data;

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
#ifdef DEBUG
  /*********To Send AT Commands through Serial monitor**********/
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

    if (msg.indexOf("CGPSINFO") > 0)
    {
      char buf[512];
      msg.toCharArray(buf, msg.length());
      char *token = strtok(buf, ":");
      token = strtok(NULL, ":");
      data.Latitude = atof(strtok(token, ","));
      data.Hem = strtok(NULL, ",");
      data.Longitude = atof(strtok(NULL, ","));
#ifdef DEBUG
      Serial.printf("\t\t %s \nLatitude: %10.6f \nLongitude: %11.6f", data.Hem, data.Latitude, data.Longitude);
#endif
    }
    msg = "";
    stringComplete = false;
  }
}