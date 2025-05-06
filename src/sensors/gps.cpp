#include <Arduino.h>
#include <UbxGps.h>
#include <SD.h>
#include <SPI.h>

#define GPS_SERIAL Serial1         // Teensy 4.1: Pin 0 = RX1, Pin 1 = TX1
#define GPS_BAUDRATE 115200

#define SD_CS_PIN BUILTIN_SDCARD   // Teensy 4.1 has built-in SD slot
#define LOG_FILENAME "gps_log.csv"

UbxGps<HardwareSerial> gps(GPS_SERIAL);
File logFile;

void setup() {
  Serial.begin(115200);
  GPS_SERIAL.begin(GPS_BAUDRATE);
  
  // Initialize GPS
  gps.begin();
  gps.setNavPvtRate(100);  // 10 Hz update rate

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    while (1);  // Halt
  }
  
  // Open or create the log file
  logFile = SD.open(LOG_FILENAME, FILE_WRITE);
  if (!logFile) {
    Serial.println("Failed to open log file!");
    while (1);
  }

  // Write CSV header
  logFile.println("Latitude_x10e7,Longitude_x10e7");
  logFile.flush();

  Serial.println("GPS and SD logging started.");
}

void loop() {
  gps.update();  // Poll GPS

  if (gps.navPvtValid()) {
    int32_t lat = gps.navPvt().lat;  // Latitude in degrees * 10^7
    int32_t lon = gps.navPvt().lon;  // Longitude in degrees * 10^7

    // Log to serial
    Serial.print("Lat: ");
    Serial.print(lat);
    Serial.print(", Lon: ");
    Serial.println(lon);

    // Log to SD
    logFile.print(lat);
    logFile.print(',');
    logFile.println(lon);
    logFile.flush();  // Ensure data is written immediately
  }
}
