#include <Wire.h>

// Function declaration
void scanI2CBus(TwoWire &wire, const char* busName, const char* pins);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("Teensy 4.1 I2C Scanner for all 3 buses");
  Serial.println("=====================================");
  
  // Initialize all I2C buses
  Wire.begin();        // I2C0 (pins 18 SDA, 19 SCL)
  Wire1.begin();       // I2C1 (pins 17 SDA, 16 SCL)  
  Wire2.begin();       // I2C2 (pins 25 SDA, 24 SCL)
  
  delay(1000);
}

void loop() {
  Serial.println("\n--- Starting I2C Bus Scan ---");
  
  // Scan I2C Bus 0 (Wire)
  scanI2CBus(Wire, "I2C0 (Wire)", "SDA=18, SCL=19");
  
  // Scan I2C Bus 1 (Wire1) 
  scanI2CBus(Wire1, "I2C1 (Wire1)", "SDA=17, SCL=16");
  
  // Scan I2C Bus 2 (Wire2)
  scanI2CBus(Wire2, "I2C2 (Wire2)", "SDA=25, SCL=24");
  
  Serial.println("\nScan complete. Next scan in 5 seconds...\n");
  delay(5000);
}

void scanI2CBus(TwoWire &wire, const char* busName, const char* pins) {
  byte error, address;
  int deviceCount = 0;
  
  Serial.print(busName);
  Serial.print(" (");
  Serial.print(pins);
  Serial.println("):");
  
  for (address = 1; address < 127; address++) {
    wire.beginTransmission(address);
    error = wire.endTransmission();
    
    if (error == 0) {
      Serial.print("  Device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.print(" (");
      Serial.print(address);
      Serial.println(")");
      deviceCount++;
    }
    else if (error == 4) {
      Serial.print("  Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  if (deviceCount == 0) {
    Serial.println("  No devices found");
  } else {
    Serial.print("  Total devices found: ");
    Serial.println(deviceCount);
  }
  Serial.println();
}