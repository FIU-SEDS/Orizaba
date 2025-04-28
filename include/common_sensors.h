#pragma once
#ifndef COMMON_SENSORS_H
#define COMMON_SENSORS_H
#include <cstdint>

#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Sensor library

// Pin address and locations
constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;   // I2C address for ASM330 Backup IMU
constexpr uint8_t MAIN_IMU_ADDRESS = 0x30;     // I2C address for BNO055 Main IMU
constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30; // I2C address for magnetometer
constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;
constexpr uint32_t BAUD_RATE = 115200; // Serial communication speed

// Sensor parameters
constexpr int8_t MAGNETOMETER_LOWER_TEMP = -40;
constexpr uint8_t MAGNETOMETER_UPPER_TEMP = 105;

// Connects to specific I2C address and calls on it to receive a response back
bool is_device_connected(uint8_t address);

// Function that initilizes magnetometer and checks temperature and I2C address
bool power_on_magnetometer();

// Verifies that the magnetometer internal temperature is between -40 and 105 degress Celsius
bool verify_magnetometer_temperature(int16_t mag_temp_reading);

#endif