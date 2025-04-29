#ifndef COMMON_SENSORS_H
#define COMMON_SENSORS_H
#include <cstdint>

#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Sensor Library
#include <Adafruit_Sensor.h>                    // Main IMU Library
#include <Adafruit_BNO055.h>                    // Main IMU Library
#include <Adafruit_HTU21DF.h>                   // Temperature and Humidity Sensor Library
#include <ASM330LHHSensor.h>                    // Backup IMU Library
#include <MS5611.h>                             // Barometer Library

// Pin address and locations
constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;     // I2C address for ASM330 Backup IMU
constexpr uint8_t MAIN_IMU_ADDRESS = 0x30;       // I2C address for BNO055 Main IMU
constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30;   // I2C address for magnetometer
constexpr uint8_t TEMP_AND_HUMID_ADDRESS = 0x40; // I2C address for HTU20DF Temperature & Humidity
constexpr uint8_t BAROMETER_ADDRESS = 0x77;      // I2C address for MS5611 Barometer
constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;
constexpr uint32_t BAUD_RATE = 115200; // Serial communication speed

// Sensor parameters
constexpr int8_t COMMON_LOWER_TEMP = -40;         // degress Celsius
constexpr uint8_t MAGNETOMETER_UPPER_TEMP = 105;  // degress Celsius
constexpr uint8_t BAROMETER_UPPER_TEMP = 85;      // degress Celsius
constexpr uint8_t TEMP_HUMIDITY_UPPER_TEMP = 125; // degress Celsius

// Connects to specific I2C address and calls on it to receive a response back
bool is_device_connected(uint8_t address);

// Function that initilizes magnetometer and checks temperature and I2C address
bool power_on_magnetometer();

bool power_on_barometer();

bool power_on_temp_and_humidity();

bool power_on_backup_IMU();

bool process_magnetometer();

#endif