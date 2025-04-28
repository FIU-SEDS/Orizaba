#pragma once
#ifndef COMMON_SENSORS_H
#define COMMON_SENSORS_H
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Sensor library

// Pin address and locations
extern constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;   // I2C address for ASM330 Backup IMU
extern constexpr uint8_t MAIN_IMU_ADDRESS = 0x30;     // I2C address for BNO055 Main IMU
extern constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30; // I2C address for magnetometer
extern constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;
extern constexpr uint32_t BAUD_RATE = 115200; // Serial communication speed

// Sensor Parameters
extern constexpr int8_t MAGNETOMETER_LOWER_TEMP = -40;
extern constexpr uint8_t MAGNETOMETER_UPPER_TEMP = 105;

// Sensor objects
SFE_MMC5983MA magnetometer; // object

// Connects to specific I2C address and calls on it to receive a response back
bool is_device_connected(uint8_t address);

// Function that initilizes magnetometer and checks temperature and I2C address
bool power_magnetometer();

// Verifies that the magnetometer internal temperature is between -40 and 105 degress Celsius
bool verify_magnetometer_temperature();

#endif