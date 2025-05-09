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

constexpr float GRAVITY_CONSTANT = 9.8;           // measured in m/s^2
constexpr uint16_t MG_TO_G = 1000;                // conversion factor for microgravity (mg) to gravity (g)
constexpr uint8_t INVALID_GRAVITY_THRESHOLD = 30; // 30Gs max ceiling for sensor if not considered faulty
constexpr uint8_t AXIS_SIZE = 3;

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

float get_tilt_angle(sensors_event_t gravity)
{

    // Calculate rocket attitude relative to ground
    // Dot product of gravity vector with rocket's Z axis gives tilt angle
    float rocket_vertical_axis[3] = {0, 0, 1}; // Assuming Z is rocket's long axis
    float gravity_vec[3] = {gravity.acceleration.x,
                            gravity.acceleration.y,
                            gravity.acceleration.z};

    // Normalize gravity vector
    float gravity_magnitude = sqrt(pow(gravity_vec[0], 2) +
                                   pow(gravity_vec[1], 2) +
                                   pow(gravity_vec[2], 2));

    for (int i = 0; i < 3; i++)
    {
        gravity_vec[i] /= gravity_magnitude;
    }

    // Calculate tilt using dot product (1 = aligned, 0 = perpendicular)
    float tilt_alignment = gravity_vec[2];                // Dot product simplified for this case
    float tilt_angle = acos(tilt_alignment) * (180 / PI); // Convert to degrees

    return tilt_angle;
}

float get_z_g_force(sensors_event_t main_IMU_accelerometer, int32_t backup_IMU_accelerometer[])
{
    float combined_z_g_forces;
    // calculating g-forces
    float main_gravity_z = main_IMU_accelerometer.acceleration.z / GRAVITY_CONSTANT; // from m/s^2 to G force
    float backup_gravity_z = static_cast<float>(backup_IMU_accelerometer[2]) / MG_TO_G;

    // Check if readings are valid
    bool main_valid = !isnan(main_gravity_z) && abs(main_gravity_z) < INVALID_GRAVITY_THRESHOLD;
    bool backup_valid = !isnan(backup_gravity_z) && abs(backup_gravity_z) < INVALID_GRAVITY_THRESHOLD;

    if (main_valid && backup_valid)
    {
        combined_z_g_forces = (main_gravity_z + backup_gravity_z) / 2.0 // average of combined readings
    }
    else if (main_valid)
    {
        combined_z_g_forces = main_gravity_z;
    }
    else
    {
        combined_z_g_forces = backup_gravity_z;
    }

    return combined_z_g_forces;
}

float get_total_g_forces(sensors_event_t main_IMU_accelerometer, int32_t backup_IMU_accelerometer[])
{
    float total_g_main = sqrt(
                             pow(main_IMU_accelerometer.acceleration.x, 2) +
                             pow(main_IMU_accelerometer.acceleration.y, 2) +
                             pow(main_IMU_accelerometer.acceleration.z, 2)) /
                         GRAVITY_CONSTANT;

    float total_g_backup = sqrt(
                               pow(backup_IMU_accelerometer[0], 2) +
                               pow(backup_IMU_accelerometer[1], 2) +
                               pow(backup_IMU_accelerometer[2], 2)) /
                           MG_TO_G; // convert from mg to G

    float total_g_forces = (total_g_main + total_g_backup) / 2.0; // total G-force (average)

    return total_g_forces;
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
    sensors_event_t angluar_velocity, linear_accleration, main_IMU_accelerometer, gravity; // typedef datatype for main IMU
    // Arrays to hold accelerometer readings
    int32_t backup_IMU_accelerometer[AXIS_SIZE] = {}; // For current acceleration reading (x, y, z)
    int32_t backup_IMU_gyroscope[AXIS_SIZE] = {};     // For current gyroscope reading (x, y, z)

    main_IMU.getEvent(&angluar_velocity, Adafruit_BNO055::VECTOR_GYROSCOPE);
    main_IMU.getEvent(&linear_accleration, Adafruit_BNO055::VECTOR_LINEARACCEL);
    main_IMU.getEvent(&main_IMU_accelerometer, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    main_IMU.getEvent(&gravity, Adafruit_BNO055::VECTOR_GRAVITY);

    backup_IMU.Get_X_Axes(backup_IMU_accelerometer);
    backup_IMU.Get_G_Axes(backup_IMU_gyroscope);

    float z_axis_g_force = get_z_g_force(main_IMU_accelerometer, backup_IMU_accelerometer); // used to measure flight
    float total_g_force = get_total_g_forces(main_IMU_accelerometer, backup_IMU_accelerometer);
    float tilt_angle = get_tilt_angle(gravity);

    // MISSING Z_AXIS_ACCEL, TOTAL ACCEL, VELOCITY, TOTAL VELO
}