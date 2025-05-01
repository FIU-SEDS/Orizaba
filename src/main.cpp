#include <state_manager.h>
#include <common_sensors.h>

void setup()
{
  Serial.begin(BAUD_RATE);
  Wire.begin();

  pinMode(13, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
}