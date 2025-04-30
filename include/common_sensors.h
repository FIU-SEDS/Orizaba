#ifndef COMMON_SENSORS_H
#define COMMON_SENSORS_H
#include <cstdint>

#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h> // Magnetometer Sensor Library
#include <Adafruit_Sensor.h>                    // Main IMU Library
#include <Adafruit_BNO055.h>                    // Main IMU Library
#include <utility/imumaths.h>                   // Main IMU orientation Library
#include <Adafruit_HTU21DF.h>                   // Temperature and Humidity Sensor Library
#include <ASM330LHHSensor.h>                    // Backup IMU Library
#include <MS5611.h>                             // Barometer Library
#include <MCP7940.h>                            // Real Time Clock Library

// Pin address and locations
constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;     // I2C address for ASM330 Backup IMU
constexpr uint8_t MAIN_IMU_ADDRESS = 0x28;       // I2C address for BNO055 Main IMU
constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30;   // I2C address for magnetometer
constexpr uint8_t TEMP_AND_HUMID_ADDRESS = 0x40; // I2C address for HTU20DF Temperature & Humidity
constexpr uint8_t BAROMETER_ADDRESS = 0x77;      // I2C address for MS5611 Barometer
constexpr uint8_t REAL_TIME_CLOCK_ADDRESS = 0x6F;
constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;
constexpr uint32_t BAUD_RATE = 115200; // Serial communication speed

// Sensor parameters
constexpr int8_t COMMON_LOWER_TEMP = -40;         // degress Celsius (multiple sensors share this lower tempearture operating range)
constexpr uint8_t MAGNETOMETER_UPPER_TEMP = 105;  // degress Celsius
constexpr uint8_t COMMON_UPPER_TEMP = 85;         // degress Celsius (multiple sensors share this higher tempearture operating range)
constexpr uint8_t TEMP_HUMIDITY_UPPER_TEMP = 125; // degress Celsius

/**
 * @brief Checks if a device at the specified I2C address is responsive
 * @param 8-bit address The I2C address to check
 * @return true if the device responds to I2C request, false otherwise
 */
bool is_device_connected(uint8_t address);

/**
 * @brief Initializes the MMC5983MA magnetometer and performs startup checks
 * @details Initializes the sensor, performs a soft reset, verifies I2C connectivity,
 *          and checks that temperature is within operating range
 * @return true if initialization and all checks pass, false otherwise
 */
bool power_on_magnetometer();

/**
 * @brief Initializes the MS5611 barometer and performs startup checks
 * @details Initializes the sensor, verifies I2C connectivity, and checks that
 *          temperature is within operating range
 * @return true if initialization and all checks pass, false otherwise
 */
bool power_on_barometer();

/**
 * @brief Initializes the HTU21DF temperature and humidity sensor and performs startup checks
 * @details Initializes the sensor, verifies I2C connectivity, and checks that
 *          temperature is within operating range
 * @return true if initialization and all checks pass, false otherwise
 */
bool power_on_temp_and_humidity();

/**
 * @brief Initializes the ASM330LHH backup IMU and performs startup checks
 * @details Initializes the sensor, enables accelerometer and gyroscope components,
 *          and verifies I2C connectivity
 * @return true if initialization and all checks pass, false otherwise
 */
bool power_on_backup_IMU();

/**
 * @brief Initializes the BNO055 main IMU and performs startup checks
 * @details Initializes the sensor, configures it to use external crystal for better accuracy,
 *          verifies I2C connectivity, and checks that temperature is within operating range
 * @return true if initialization and all checks pass, false otherwise
 */
bool power_on_main_IMU();

/**
 * @brief Initializes the MCP7940 Real Time Clock and performs startup checks
 * @details Initilizes sensor, checks the status of the device and ensures that the oscillator is running,
 *          verifies I2C connection, and sets to current time
 * @return true if initialization and all checks pass, fail otherwise
 * */
bool power_on_real_time_clock();

#endif