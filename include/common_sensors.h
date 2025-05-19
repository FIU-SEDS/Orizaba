#ifndef COMMON_SENSORS_H
#define COMMON_SENSORS_H
#include <cstdint>
#include <Wire.h>

// Pin address and locations
constexpr uint8_t BACKUP_IMU_ADDRESS = 0x6A;     // I2C address for ASM330 Backup IMU
constexpr uint8_t MAIN_IMU_ADDRESS = 0x28;       // I2C address for BNO055 Main IMU
constexpr uint8_t MAGNETOMETER_ADDRESS = 0x30;   // I2C address for magnetometer
constexpr uint8_t TEMP_AND_HUMID_ADDRESS = 0x40; // I2C address for HTU20DF Temperature & Humidity
constexpr uint8_t BAROMETER_ADDRESS = 0x77;      // I2C address for MS5611 Barometer
constexpr uint8_t REAL_TIME_CLOCK_ADDRESS = 0x6F;

// I2C pins are memory mapped: we specify a memory address
// For SPI we need to specify the pin that the component connects to on the controller
constexpr uint8_t SD_CARD_MODULE_CS_PIN = 10;
constexpr uint8_t FLASH_CARD_CS_PIN = 4; // FIGURE OUT

constexpr uint32_t BAUD_RATE = 115200; // Serial communication speed

// Sensor parameters
constexpr int8_t COMMON_LOWER_TEMP = -40;         // degress Celsius (multiple sensors share this lower tempearture operating range)
constexpr uint8_t MAGNETOMETER_UPPER_TEMP = 105;  // degress Celsius
constexpr uint8_t COMMON_UPPER_TEMP = 85;         // degress Celsius (multiple sensors share this higher tempearture operating range)
constexpr uint8_t TEMP_HUMIDITY_UPPER_TEMP = 125; // degress Celsius

enum sensors : uint8_t // this is to be used as the PACKET IDs when sending data over radio!
{
  BAROMETER,
  IMUS,
  MAGNETOMETER,
  REAL_TIME_CLOCK,
  TEMP,
  HUMID,
  GPS,
  SENSORS_AMOUNT
};

enum sensor_readouts : uint8_t // this is to be used as the index for the global array!
{
  AVERAGE_ACCELERATION_X,
  AVERAGE_ACCELERATION_Y,
  AVERAGE_ACCELERATION_Z,
  ANGULAR_VELOCITY_X,
  ANGULAR_VELOCITY_Y,
  ANGULAR_VELOCITY_Z,
  TILT_ANGLE,
  Z_AXIS_G_FORCE,
  TOTAL_G_FORCE,
  LINEAR_ACCELERATION_X,
  LINEAR_ACCELERATION_Y,
  LINEAR_ACCELEARTION_Z,
  LINEAR_VELOCITY_X,
  LINEAR_VELOCITY_Y,
  LINEAR_VELOCITY_Z,
  PRESSURE,    // barometer in pascals
  ALTITUDE,    // barometer in feet
  TEMPERATURE, // temp
  HUMIDITY,    // humid
  HEADING,     // magnetometer
  LATITUDE,    // GPS
  LONGITUDE,
  SENSOR_READOUTS_AMOUNT
};

// extern float global_sensor_vals[SENSORS_AMOUNT]; // defunct! We want a global array of all the sensor readouts.
extern float global_sensor_vals[SENSOR_READOUTS_AMOUNT];

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

bool power_on_GPS();


bool process_temp_and_humidity();
bool process_barometer();
bool process_IMUs();
bool process_magnetometer();
bool process_real_time_clock();
bool power_on_lora_998();
bool power_on_storage();


#endif
