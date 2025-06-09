#include <Wire.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <Arduino.h>

void setup()
{
  Wire.begin();       // Join I2C bus
  Serial.begin(115200); // Start serial communication
  while (!Serial)
    ; // Wait for serial monitor
  Serial.println("\nI2C Scanner");
}

void loop()
{
  byte error, address;
  int devicesFound = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");

      devicesFound++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (devicesFound == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("Done\n");

  delay(5000); // Wait 5 seconds for next scan
}