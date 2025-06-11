//
//    FILE: MS5611_minimal_Wire2.ino
//  AUTHOR: Rob Tillaart (modified for Wire2)
// PURPOSE: demo application using Wire2 on Teensy 4.1
//    DATE: 2021-12-24
//     URL: https://github.com/RobTillaart/MS5611

#include <MS5611.h>

//  BREAKOUT  MS5611  aka  GY63 - see datasheet
//
//  SPI    I2C
//              +--------+
//  VCC    VCC  | o      |
//  GND    GND  | o      |
//         SCL  | o      |  <- Connect to SCL2 (pin 25) on Teensy 4.1
//  SDI    SDA  | o      |  <- Connect to SDA2 (pin 24) on Teensy 4.1
//  CSO         | o      |
//  SDO         | o L    |   L = led
//          PS  | o    O |   O = opening  PS = protocol select
//              +--------+
//
//  PS to VCC  ==>  I2C  (GY-63 board has internal pull up, so not needed)
//  PS to GND  ==>  SPI
//  CS to VCC  ==>  0x76
//  CS to GND  ==>  0x77

// Create MS5611 object with Wire2 interface
MS5611 barometer(0x77, &Wire2);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("MS5611_LIB_VERSION: ");
  Serial.println(MS5611_LIB_VERSION);
  Serial.println();
  
  // Initialize Wire2 instead of Wire
  Wire2.begin();
  
  if (barometer.begin() == true)
  {
    Serial.print("MS5611 found on Wire2: ");
    Serial.println(barometer.getAddress());
  }
  else
  {
    Serial.println("MS5611 not found on Wire2. halt.");
    while(1); // Halt execution if sensor not found
  }
  Serial.println();
}

void loop()
{
  barometer.read(); //  note no error checking => "optimistic".
  Serial.print("T:\t");
  Serial.print(barometer.getTemperature(), 2);
  Serial.print("\tP:\t");
  Serial.print(barometer.getPressure(), 2);
  Serial.println();
  delay(1000);
}

//  -- END OF FILE --