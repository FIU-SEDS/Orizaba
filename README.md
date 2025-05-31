# ORIZABA FLIGHT COMPUTER CODE

SEDS Avionics Team Full-Scale Flight Computer 2024-2025 Season

**Microcontroller**: Teensy 4.1

**LIST FOR SENSOR DATA STORAGE/TRANSMISSION** 
- [Google Sheets Link](https://docs.google.com/spreadsheets/d/15xEbT6rAZYMg4pbxv4v-uwzvX4tvr4YyPcp8unlj1kQ/edit?usp=sharing)

---

## Sensors:

#### BNO055 Main IMU

- [Github Library](https://github.com/adafruit/Adafruit_BNO055)
- [Datasheet](https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/downloads)

#### ASM330LHH Backup IMU

- [Github Library](https://github.com/stm32duino/ASM330LHH/tree/main)
- [Datasheet](https://www.st.com/resource/en/datasheet/asm330lhh.pdf)
- [Wiring Diagram](https://github.com/user-attachments/assets/08a4da38-90d2-46dd-91a4-b4015cfabe34)

#### MS561101BA03-50 Barometer

- [Github Library](https://github.com/RobTillaart/MS5611)
- [Datasheet](https://www.mouser.fr/ProductDetail/Measurement-Specialties/MS561101BA03-50?qs=%2BgKeJhng5iV%2BnCz6Qd5iDw%3D%3D)

#### MAX-M8C-0 GPS

- [Github Library](https://github.com/u-blox/ubxlib) **PENDING**
- [Datasheet](https://www.digikey.co.nz/en/products/detail/u-blox/MAX-M8C-0/6150635)

#### MMC5983MA Magnetometer

- [Datasheet]()**PENDING**
- [Github Library](https://github.com/sparkfun/SparkFun_MMC5983MA_Magnetometer_Arduino_Library)

#### HTU20DF Temperature & Humidity Sensor

- [Datasheet](https://www.farnell.com/datasheets/2305659.pdf)
- [Github Library](https://github.com/adafruit/Adafruit_HTU21DF_Library?tab=readme-ov-file)

#### MCP7940M-I/SN Real-time Clock

- [Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/20002292B.pdf)
- [Github Library](https://github.com/Zanduino/MCP7940/tree/master)

---

## Radios:

#### Reyax 868/915 MHz LoRa RYLR998 module

- [Website](https://reyax.com//products/RYLR998)
- [AT Command Manual](https://reyax.com//upload/products_download/download_file/LoRa_AT_Command_RYLR998_RYLR498_EN.pdf)
- [Datasheet](https://reyax.com//upload/products_download/download_file/RYLR998_EN.pdf)

#### Reyax RYLR 689 LoRa module

- [Website](https://reyax.com//products/RYLR689)
- [Datasheet](https://reyax.com//upload/products_download/download_file/RYLR689.pdf)

---

## Storage:

#### SST26VF080AT-104I/SN Flash IC Memory

- [Github Library](https://github.com/Marzogh/SPIMemory)

#### SD Card Module

- Standard Arduino SPI SD card library

---

### Sensor I2C Addresses:

- Main IMU (BNO055): `0x28`
- Backup IMU (ASM330): `0x6A`
- Magnetometer: `0x30`
- Temperature & Humidity Sensor (HTU20D): `0x40`
- Barometer (MS5611): `0x77`

---

### LESSONS learned from VINSON FLIGHT COMPUTER SUBSCALE:

- **IGNORE .PIO Folder:** ADD the `.pio` to the .gitignore to not flood the git repository and add any sensor functions from the sensor library to a seperate header file

---

### Branch Creation Template

#### Describe your issue

Please provide a detailed description of the issue you are reporting.

#### Checklist before creating a branch
- [ ] I have created a new branch from the `main` branch.
- [ ] My branch name starts with either `fix/` or `feature/` followed by a descriptive name.
- [ ] I have committed my changes and pushed them to the branch.
- [ ] I have performed a self-review of my code and tested it on my branch.
- [ ] I have deleted the branch after the pull request is merged into `main`

---

### GUIDELINES for Setting up Platform.IO

- **Single Main File:** Ensure there is only **ONE** `.cpp` file that contains `setup()` and `loop()` in your `src` directory. Multiple files with these functions will cause compilation errors.
- **Archive Storage:** If files are not intended for compilation, place them in the `archive` folder since only the `src` folder is compiled.
- **Test Folder Usage:** Unless you are actively testing, **do not** place any files in the `test` folder as it may cause errors.
- **Include Folder:** Any custom header files should be placed in the `include` folder, which is recognized by PlatformIO as the location for header files.

---

# Commenting Standards

This project uses C, C++, and Arduino code for embedded systems, and we adhere to a consistent commenting style to keep the codebase clear, maintainable, and easy to navigate.

## 1. Function-Level Comments

- **Format:** Use Doxygen-style comments (`/** ... */`) before each function.
- **Content:**
  - Briefly describe the function’s purpose, parameters, and return values.
  - Use `@brief` to summarize the function at a high level.
  - Use `@param` and `@return` tags to document function inputs/outputs.
  - For any known issues, planned work, or bugs, use `TODO(#issueNumber)` or `FIXME(#issueNumber)`.
  - Use `NOTE(name):` to highlight non-actionable observations or temporary conditions.

**Example:**

```c
/**
 * @brief Fetches data from a sensor and processes the result.
 *
 * Reads from the specified sensor pin, applies filtering, and returns the
 * processed value. This function is non-blocking.
 *
 * @param sensorPin The Arduino analog pin number where the sensor is connected.
 * @return The filtered sensor reading as an integer.
 *
 * NOTE(alice): Currently using a simple moving average filter. See #45 for a discussion on implementing a Kalman filter.
 * TODO(#101): Integrate a calibration routine to improve accuracy.
 * FIXME(#102): Handle sensor saturation conditions more gracefully.
 */
int readAndProcessSensor(int sensorPin) {
    // ...
}
```

## 2. Single-Level Comments (Above or Inline)

- **Format:** Use Doxygen-style comments (`//`) before each comment.
- **Content:**
  - Briefly describe the purpose of the code
  - (Case-by-case basis) Tag anyone who will work on this code

```c
/// This is a single-line Doxygen comment.
/// It documents myOtherVariable.
int myOtherVariable;
```

```c
int myVariable // Place the comment here
```

## 3. TODO/FIXME Comments

Use `TODO(#issueNumber)`or `FIXME(#issueNumber)` to mark pending tasks, enhancements, or additional steps you plan to implement.
**Conditions:**

- If going to use in a function multi-line comment you can place it inside as noted in #1 Function-Level Comments
- If going to be single line then mark it as the following below:

```c
int calculateChecksum(uint8_t data[], size_t length) {
    int checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i]; // TODO(#789): Handle overflow for large data arrays.
    }
    return checksum;
}
```

or

```c
bool connectToWiFi(const char* ssid, const char* password) {
    bool success = WiFi.begin(ssid, password); // FIXME(#456): Implement retry logic for failed connections.
    return success;
}
```

- If needed for a multi-line `TODO/FIXME` comment then use the syntax as following to not clutter the code and hinder readbility:

```c
/**
 * FIXME(#456): Implement retry logic for failed connections.
 */
bool connectToWiFi(const char* ssid, const char* password) {
    bool success = WiFi.begin(ssid, password);
    return success;
}

```
