#include <common_sensors.h>
#include <io.h>
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

constexpr float GRAVITY_CONSTANT = 9.80665;                      // measured in m/s^2
constexpr uint16_t MG_TO_G = 1000;                               // conversion factor for microgravity (mg) to gravity (g)
constexpr uint8_t INVALID_GRAVITY_THRESHOLD = 30;                // 30Gs max ceiling for sensor if not considered faulty
constexpr uint8_t INVALID_ROTATIONAL_THRESHOLD = 12;             // reason for 12 rad/s thereshold is only to detect catastrophic tumbling not minor spins
constexpr float DEG_TO_RAD_PER_SECOND = (M_PI / 180.0) / 1000.0; // conversion factor for degrees per second to radians per second
constexpr uint8_t AXIS_SIZE = 3;

// Sensor objects
Adafruit_BNO055 main_IMU = Adafruit_BNO055(55, 0x28);      // main IMU BNO055 object
ASM330LHHSensor backup_IMU(&DEV_I2C, ASM330LHH_I2C_ADD_L); // backup IMU ASM330LHH object

// function to get seconds in epoch time from real time clock file
extern int get_epoch_seconds();

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
    /*
    Dot product of gravity vector with rocket's Z axis gives tilt angle float rocket_vertical_axis[3] = {0, 0, 1}; assuming Z is rocket's long axis here is the formula: dot_product = 0*gravity_vec[0] + 0*gravity_vec[1] + 1*gravity_vec[2] = gravity_vec[2]
    */

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
        combined_z_g_forces = (main_gravity_z + backup_gravity_z) / 2.0; // average of combined readings
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

// gets the average angular velocity (rads/s) from main and backup IMUs
void get_average_angular_velocity(sensors_event_t angular_velocity, int32_t backup_IMU_gyroscope[], float &avg_gyro_x, float &avg_gyro_y, float &avg_gyro_z)
{
    // main IMU values (already in rad/s)
    float main_gyro_x = angular_velocity.gyro.x;
    float main_gyro_y = angular_velocity.gyro.y;
    float main_gyro_z = angular_velocity.gyro.z;

    // converting backup IMU from deg to rad/s
    float backup_gyro_x_rad = static_cast<float>(backup_IMU_gyroscope[0]) * DEG_TO_RAD_PER_SECOND;
    float backup_gyro_y_rad = static_cast<float>(backup_IMU_gyroscope[1]) * DEG_TO_RAD_PER_SECOND;
    float backup_gyro_z_rad = static_cast<float>(backup_IMU_gyroscope[2]) * DEG_TO_RAD_PER_SECOND;

    // verifying if readings are valid
    bool main_valid_x = !isnan(main_gyro_x) && abs(main_gyro_x) < INVALID_ROTATIONAL_THRESHOLD;
    bool main_valid_y = !isnan(main_gyro_y) && abs(main_gyro_y) < INVALID_ROTATIONAL_THRESHOLD;
    bool main_valid_z = !isnan(main_gyro_z) && abs(main_gyro_z) < INVALID_ROTATIONAL_THRESHOLD;

    bool backup_valid_x = !isnan(backup_gyro_x_rad) && abs(backup_gyro_x_rad) < INVALID_ROTATIONAL_THRESHOLD;
    bool backup_valid_y = !isnan(backup_gyro_y_rad) && abs(backup_gyro_y_rad) < INVALID_ROTATIONAL_THRESHOLD;
    bool backup_valid_z = !isnan(backup_gyro_z_rad) && abs(backup_gyro_z_rad) < INVALID_ROTATIONAL_THRESHOLD;

    // calculating average for x-axis between both IMUs
    if (main_valid_x && backup_valid_x)
    {
        avg_gyro_x = (main_gyro_x + backup_gyro_x_rad) / 2.0;
    }
    else if (main_valid_x)
    {
        avg_gyro_x = main_gyro_x;
    }
    else
    {
        avg_gyro_x = backup_gyro_x_rad;
    }
    // calculating average for y-axis between both IMUs
    if (main_valid_y && backup_valid_y)
    {
        avg_gyro_y = (main_gyro_y + backup_gyro_y_rad) / 2.0;
    }
    else if (main_valid_y)
    {
        avg_gyro_y = main_gyro_y;
    }
    else
    {
        avg_gyro_y = backup_gyro_y_rad;
    }

    // calculating average for z-axis between both IMUs
    if (main_valid_z && backup_valid_z)
    {
        avg_gyro_z = (main_gyro_z + backup_gyro_z_rad) / 2.0;
    }
    else if (main_valid_z)
    {
        avg_gyro_z = main_gyro_z;
    }
    else
    {
        avg_gyro_z = backup_gyro_z_rad;
    }
}

void get_average_acceleration(sensors_event_t main_IMU_accelerometer, int32_t backup_IMU_accelerometer[], float &avg_accel_x, float &avg_accel_y, float &avg_accel_z)
{
    // main IMU values (in m/s^2)
    float main_accel_x = main_IMU_accelerometer.acceleration.x;
    float main_accel_y = main_IMU_accelerometer.acceleration.y;
    float main_accel_z = main_IMU_accelerometer.acceleration.z;

    // backup IMU readings are in milligravity converted to m/s^2
    float backup_accel_x = (static_cast<float>(backup_IMU_accelerometer[0]) / MG_TO_G) * GRAVITY_CONSTANT;
    float backup_accel_y = (static_cast<float>(backup_IMU_accelerometer[1]) / MG_TO_G) * GRAVITY_CONSTANT;
    float backup_accel_z = (static_cast<float>(backup_IMU_accelerometer[2]) / MG_TO_G) * GRAVITY_CONSTANT;

    bool main_accel_valid_x = !isnan(main_accel_x) && abs(main_accel_x) < 10.0;
    bool main_accel_valid_y = !isnan(main_accel_y) && abs(main_accel_y) < 10.0;
    bool main_accel_valid_z = !isnan(main_accel_z) && abs(main_accel_z) < 10.0;

    bool backup_accel_valid_x = !isnan(backup_accel_x) && abs(backup_accel_x) < 10.0;
    bool backup_accel_valid_y = !isnan(backup_accel_y) && abs(backup_accel_y) < 10.0;
    bool backup_accel_valid_z = !isnan(backup_accel_z) && abs(backup_accel_z) < 10.0;

    // calculating average for x-axis between both IMUs
    if (main_accel_valid_x && backup_accel_valid_x)
    {
        avg_accel_x = (main_accel_x + backup_accel_x) / 2.0;
    }
    else if (main_accel_valid_x)
    {
        avg_accel_x = main_accel_x;
    }
    else
    {
        avg_accel_x = backup_accel_x;
    }
    // calculating average for y-axis between both IMUs
    if (main_accel_valid_y && backup_accel_valid_y)
    {
        avg_accel_y = (main_accel_y + backup_accel_y) / 2.0;
    }
    else if (main_accel_valid_y)
    {
        avg_accel_y = main_accel_valid_y;
    }
    else
    {
        avg_accel_y = backup_accel_y;
    }

    // calculating average for z-axis between both IMUs
    if (main_accel_valid_z && backup_accel_valid_z)
    {
        avg_accel_z = (main_accel_z + backup_accel_z) / 2.0;
    }
    else if (main_accel_valid_z)
    {
        avg_accel_z = main_accel_z;
    }
    else
    {
        avg_accel_z = backup_accel_z;
    }
}

void get_linear_velocity(float linear_accel_x, float linear_accel_y, float linear_accel_z, float &linear_velocity_x, float &linear_velocity_y, float &linear_velocity_z)
{
    static float prev_velocity_x = 0.0, prev_velocity_y = 0.0, prev_velocity_z = 0.0;
    static int previous_time = 0;
    static bool first_iteration = true;

    // get current time
    int current_time = get_epoch_seconds();

    // calculate linear velocity using Vx = Vox + at
    if (!first_iteration)
    {
        float delta_time = static_cast<float>(current_time - previous_time); // dt in seconds

        // applying kinematics equation for each axis: Vx = Vox + a*t
        linear_velocity_x = prev_velocity_x + (linear_accel_x * delta_time);
        linear_velocity_y = prev_velocity_y + (linear_accel_y * delta_time);
        linear_velocity_z = prev_velocity_z + (linear_accel_z * delta_time);

        // update previous velocities
        prev_velocity_x = linear_velocity_x;
        prev_velocity_y = linear_velocity_y;
        prev_velocity_z = linear_velocity_z;
    }
    else
    {
        // first iteration initializes the velocities to 0
        linear_velocity_x = linear_velocity_y = linear_velocity_z = 0.0;
        first_iteration = false;
    }
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
    sensors_event_t angular_velocity, linear_accleration, main_IMU_accelerometer, gravity;                                                    // typedef datatype for main IMU
    float avg_gyro_x, avg_gyro_y, avg_gyro_z, avg_accel_x, avg_accel_y, avg_accel_z, linear_velocity_x, linear_velocity_y, linear_velocity_z; // raw data values gyro saved in rad/s, accelerometer saved in m/s^2, and velocity saved in m/s

    // Arrays to hold backup IMU readings
    int32_t backup_IMU_accelerometer[AXIS_SIZE] = {}; // For current acceleration reading (x, y, z)
    int32_t backup_IMU_gyroscope[AXIS_SIZE] = {};     // For current gyroscope reading (x, y, z)

    main_IMU.getEvent(&angular_velocity, Adafruit_BNO055::VECTOR_GYROSCOPE);           // in rads/s
    main_IMU.getEvent(&linear_accleration, Adafruit_BNO055::VECTOR_LINEARACCEL);       // in m/s^2 minus gravity
    main_IMU.getEvent(&main_IMU_accelerometer, Adafruit_BNO055::VECTOR_ACCELEROMETER); // in m/s^2 plus gravity
    main_IMU.getEvent(&gravity, Adafruit_BNO055::VECTOR_GRAVITY);                      // in m/s^2 minus any movement
    backup_IMU.Get_X_Axes(backup_IMU_accelerometer);
    backup_IMU.Get_G_Axes(backup_IMU_gyroscope);

    float z_axis_g_force = get_z_g_force(main_IMU_accelerometer, backup_IMU_accelerometer); // used to measure flight
    float total_g_force = get_total_g_forces(main_IMU_accelerometer, backup_IMU_accelerometer);
    float tilt_angle = get_tilt_angle(gravity);
    float linear_accel_x = linear_accleration.acceleration.x;
    float linear_accel_y = linear_accleration.acceleration.y;
    float linear_accel_z = linear_accleration.acceleration.z;

    get_average_angular_velocity(angular_velocity, backup_IMU_gyroscope, avg_gyro_x, avg_gyro_y, avg_gyro_z);          // raw angular velocity data averaged out from both IMUs
    get_average_acceleration(main_IMU_accelerometer, backup_IMU_accelerometer, avg_accel_x, avg_accel_y, avg_accel_z); // raw acceleartion data averaged out from both IMUs

    get_linear_velocity(linear_accel_x, linear_accel_y, linear_accel_z, linear_velocity_x, linear_velocity_y, linear_velocity_z); // returns linear velocity in xyz axis

    // Create serial object and serialize all IMU data
    serial imu_data;
    imu_data.serialize_float(avg_accel_x);
    imu_data.serialize_float(avg_accel_y);
    imu_data.serialize_float(avg_accel_z);
    imu_data.serialize_float(avg_gyro_x);
    imu_data.serialize_float(avg_gyro_y);
    imu_data.serialize_float(avg_gyro_z);
    imu_data.serialize_float(total_g_force);
    imu_data.serialize_float(tilt_angle);
    imu_data.serialize_float(z_axis_g_force);
    imu_data.serialize_float(linear_accel_x);
    imu_data.serialize_float(linear_accel_y);
    imu_data.serialize_float(linear_accel_z);
    imu_data.serialize_float(linear_velocity_x);
    imu_data.serialize_float(linear_velocity_y);
    imu_data.serialize_float(linear_velocity_z);

    // Single call to write and transmit all IMU data
    write_and_transmit(IMUS, imu_data);

    // Update global sensor values
    global_sensor_vals[AVERAGE_ACCELERATION_X] = avg_accel_x;
    global_sensor_vals[AVERAGE_ACCELERATION_Y] = avg_accel_y;
    global_sensor_vals[AVERAGE_ACCELERATION_Z] = avg_accel_z;
    global_sensor_vals[ANGULAR_VELOCITY_X] = avg_gyro_x;
    global_sensor_vals[ANGULAR_VELOCITY_Y] = avg_gyro_y;
    global_sensor_vals[ANGULAR_VELOCITY_Z] = avg_gyro_z;
    global_sensor_vals[TOTAL_G_FORCE] = total_g_force;
    global_sensor_vals[TILT_ANGLE] = tilt_angle;
    global_sensor_vals[Z_AXIS_G_FORCE] = z_axis_g_force;
    global_sensor_vals[LINEAR_ACCELERATION_X] = linear_accel_x;
    global_sensor_vals[LINEAR_ACCELERATION_Y] = linear_accel_y;
    global_sensor_vals[LINEAR_ACCELEARTION_Z] = linear_accel_z;
    global_sensor_vals[LINEAR_VELOCITY_X] = linear_velocity_x;
    global_sensor_vals[LINEAR_VELOCITY_Y] = linear_velocity_y;
    global_sensor_vals[LINEAR_VELOCITY_Z] = linear_velocity_z;

    return true;
}