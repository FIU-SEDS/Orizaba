#include <UbxGpsConfig.h> // Library for configuring the u-blox GPS module
#include <common_sensors.h>
#include <io.h>

// Define constants for GPS
#define GPS_BAUDRATE 115200
#define GPS_DEFAULT_BAUDRATE 9600
#define GPS_ADDRESS 0x42 // Example I2C address for GPS (if applicable)

//Example GPS data
#define LATITUDE 0
#define LONGITUDE 1
#define ALTITUDE 2

// Create a GPS configuration object
    UbxGpsConfig<HardwareSerialIMXRT, usb_serial_class> *ubxGpsConfig = 
    new UbxGpsConfig<HardwareSerialIMXRT, usb_serial_class>(Serial1, Serial);

// Function to verify GPS connection
bool verify_gps_connection()
{
    // Attempt to restore GPS defaults
    ubxGpsConfig->setBaudrate(GPS_DEFAULT_BAUDRATE);
    ubxGpsConfig->configure();

    // Check if the GPS is connected (example check, modify as needed)
    if (!is_device_connected(GPS_ADDRESS))
    {
        Serial.println("GPS I2C check failed.");
        return false;
    }

    Serial.println("GPS connection verified.");
    return true;
}

// Function to initialize the GPS module
bool power_on_gps()
{
    Serial.println("Initializing GPS...");

    // Set the GPS baud rate and message type
    ubxGpsConfig->setBaudrate(GPS_BAUDRATE);
    ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
    ubxGpsConfig->setRate(100); // Set update rate to 100 ms (10 Hz)

    // Configure the GPS module
    ubxGpsConfig->configure();

    // Verify GPS connection
    if (!verify_gps_connection())
    {
        Serial.println("GPS initialization failed.");
        return false;
    }

    Serial.println("GPS initialized successfully.");
    return true;
}

// Function to process GPS data
bool process_gps()
{
    // Example: Read GPS data (latitude, longitude, altitude)
    float latitude = 0.0;  // Replace with actual GPS latitude reading
    float longitude = 0.0; // Replace with actual GPS longitude reading
    float altitude = 0.0;  // Replace with actual GPS altitude reading

    // Store and transmit GPS data
    global_sensor_vals[LATITUDE] = latitude;
    global_sensor_vals[LONGITUDE] = longitude;
    global_sensor_vals[ALTITUDE] = altitude;

    // Transmit the data
    write_and_transmit(LATITUDE, latitude);
    write_and_transmit(LONGITUDE, longitude);
    write_and_transmit(ALTITUDE, altitude);

    Serial.println("GPS data processed.");
    return true;
}

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    Serial1.begin(GPS_DEFAULT_BAUDRATE); // Start GPS communication at default baud rate

    // Power on and initialize the GPS module
    if (!power_on_gps())
    {
        Serial.println("GPS setup failed.");
        while (true); // Halt execution if GPS initialization fails
    }
}

void loop()
{
    // Process GPS data in the main loop
    if (!process_gps())
    {
        Serial.println("Failed to process GPS data.");
    }
}

void write_and_transmit(int sensorType, float value)
{
    // function to write and transmit sensor data
    Serial.print("Sensor Type: ");
    Serial.print(sensorType);
    Serial.print(", Value: ");
    Serial.println(value);

    Wire.beginTransmission(GPS_ADDRESS); // Replace with the actual I2C address
    Wire.write(sensorType);                     // Send the sensor type
    Wire.write((byte *)&value, sizeof(value));  // Send the float value as bytes
    Wire.endTransmission();
}