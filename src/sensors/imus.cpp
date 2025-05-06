#include <common_sensors.h>
#include <Adafruit_Sensor.h>  // Main IMU Library
#include <Adafruit_BNO055.h>  // Main IMU Library
#include <utility/imumaths.h> // Main IMU orientation Library
#include <ASM330LHHSensor.h>  // Backup IMU Library

#ifdef ARDUINO_SAM_DUE
#define DEV_I2C Wire1
#elif defined(ARDUINO_ARCH_STM32)
#define DEV_I2C Wire
#elif defined(ARDUINO_ARCH_AVR)
#define DEV_I2C Wire
#else
#define DEV_I2C Wire
#endif
#define SerialPort Serial

// Sensor objects
Adafruit_BNO055 main_IMU = Adafruit_BNO055(55, 0x28);      // main IMU BNO055 object
ASM330LHHSensor backup_IMU(&DEV_I2C, ASM330LHH_I2C_ADD_L); // backup IMU ASM330LHH object

bool verify_main_IMU_temperature(int8_t main_IMU_temp_reading)
{
    if (main_IMU_temp_reading < COMMON_LOWER_TEMP || main_IMU_temp_reading > MAGNETOMETER_UPPER_TEMP)
    {
        return false;
    }
    return true;
}

bool power_on_main_IMU()
{
    if (main_IMU.begin() == false)
    {
        Serial.println("Main IMU Begin function failed");
        return false;
    }

    main_IMU.setExtCrystalUse(true); // external crystal provies more reliable and percise measurements

    if (!is_device_connected(MAIN_IMU_ADDRESS))
    {
        Serial.println("Main IMU I2C check failed.");
        return false;
    }

    if (!verify_main_IMU_temperature(main_IMU.getTemp()))
    {
        Serial.println("Main IMU temperature check failed.");
        return false;
    }

    return true;
}

bool power_on_backup_IMU()
{
    if (backup_IMU.begin() == ASM330LHH_ERROR)
    {
        Serial.println("Backup IMU BEGIN function failed.");
        return false;
    }

    backup_IMU.Enable_X(); // enables accelerometer component on sensor
    backup_IMU.Enable_G(); // enables gyroscope component on sensor

    if (!is_device_connected(BACKUP_IMU_ADDRESS))
    {
        Serial.println("Backup IMU I2C check failed.");
        return false;
    }

    return true;
}

bool process_IMUs()
{
    sensors_event_t angluar_velocity, linear_accleration, accelerometer_data, gravity_data;
    main_IMU.getEvent(&angluar_velocity, Adafruit_BNO055::VECTOR_GYROSCOPE);
    main_IMU.getEvent(&linear_accleration, Adafruit_BNO055::VECTOR_LINEARACCEL);
    main_IMU.getEvent(&accelerometer_data, Adafruit_BNO055::VECTOR_ACCELEROMETER); // only in Z-axis
    main_IMU.getEvent(&gravity_data, Adafruit_BNO055::VECTOR_GRAVITY);             // only in Z-axis
}