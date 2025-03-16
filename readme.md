# MPM10 Particulate Matter Sensor Driver

The `MPM10` driver is a C++ library designed to interface with the MPM10 particulate matter sensor. This library supports both UART and I2C communication modes, providing an easy-to-use API to read particulate matter data (PM1.0, PM2.5, PM10) and particle counts in various air volumes. It is compatible with Arduino and other platforms that support the Arduino framework.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
  - [Basic Example](#basic-example)
  - [Advanced Example](#advanced-example)
- [API Reference](#api-reference)
- [Dependencies](#dependencies)
- [License](#license)
- [Author](#author)

## Features

- Supports both UART and I2C communication modes.
- Reads PM1.0, PM2.5, and PM10 concentrations in µg/m³.
- Retrieves particle counts for various air volumes (300 mL, 0.5 L, 1 L, 2.5 L, 5 L, 10 L).
- Provides measurement context switching (Standard or Atmospheric).
- Includes sensor connection status checking.
- Lightweight and efficient implementation.

## Installation

1. **Download the Library**:
   - Clone or download this repository to your local machine.

2. **Install the Library**:
   - Copy the folder containing the `MPM10.h` and related files to the `libraries` folder in your Arduino IDE sketchbook directory. For example:
     ```
     <sketchbook>/libraries/MPM10/
     ```

3. **Install Dependencies**:
   - Ensure the required dependencies (listed in the [Dependencies](#dependencies) section) are installed.

4. **Restart the Arduino IDE**:
   - Restart the Arduino IDE to make the library available under the `Sketch > Include Library` menu.

## Usage

### Basic Example

Below is a basic example of using the `MPM10` library to read PM1.0, PM2.5, and PM10 concentrations using I2C mode:

```cpp
#include <MPM10.h>

MPM10 sensor; // Default constructor for I2C mode

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open

  if (sensor.isConnected()) {
    Serial.println("MPM10 sensor connected successfully!");
  } else {
    Serial.println("Failed to connect to MPM10 sensor!");
    while (1); // Halt execution
  }
}

void loop() {
  MPM10_DATA data = sensor.readData();

  if (data.valid) {
    Serial.print("PM1.0: "); Serial.print(data.pm1_0); Serial.println(" µg/m³");
    Serial.print("PM2.5: "); Serial.print(data.pm2_5); Serial.println(" µg/m³");
    Serial.print("PM10: "); Serial.print(data.pm10); Serial.println(" µg/m³");
  } else {
    Serial.println("Invalid data received from sensor!");
  }

  delay(1000); // Wait 1 second before the next reading
}
```

### Advanced Example

This example demonstrates how to use UART mode, change the measurement context, and retrieve particle counts for different air volumes:

```cpp
#include <MPM10.h>

MPM10 sensor(17, 16); // Constructor for UART mode (TX pin 17, RX pin 16)

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial Monitor to open

  if (sensor.isConnected()) {
    Serial.println("MPM10 sensor connected successfully!");
  } else {
    Serial.println("Failed to connect to MPM10 sensor!");
    while (1); // Halt execution
  }

  // Set measurement context to atmospheric
  sensor.setMeasurementContext(MPM10_ATM_CONTEXT);
}

void loop() {
  MPM10_DATA data = sensor.readData();

  if (data.valid) {
    Serial.print("PM1.0: "); Serial.print(data.pm1_0); Serial.println(" µg/m³");
    Serial.print("PM2.5: "); Serial.print(data.pm2_5); Serial.println(" µg/m³");
    Serial.print("PM10: "); Serial.print(data.pm10); Serial.println(" µg/m³");

    // Read particle counts in various air volumes
    Serial.print("Particles in 300 mL: "); Serial.println(sensor.getPMIn300mL());
    Serial.print("Particles in 1 L: "); Serial.println(sensor.getPMIn1L());
    Serial.print("Particles in 10 L: "); Serial.println(sensor.getPMIn10L());
  } else {
    Serial.println("Invalid data received from sensor!");
  }

  delay(5000); // Wait 5 seconds before the next reading
}
```

## API Reference

### Class: `MPM10`

#### Constructors

- `MPM10()`  
  Default constructor for I2C mode.

- `MPM10(uint8_t tx, uint8_t rx)`  
  Constructor for UART mode, specifying the TX and RX pins.

#### Methods

- `MPM10_DATA readData()`  
  Reads and returns the sensor data (PM1.0, PM2.5, PM10, and validity status).

- `void setMeasurementContext(uint8_t context)`  
  Sets the measurement context. Use `MPM10_STD_CONTEXT` for standard mode or `MPM10_ATM_CONTEXT` for atmospheric mode.

- `uint16_t getPMIn300mL()`  
  Returns the number of particles >0.1 µm in 300 mL of air.

- `uint16_t getPMIn500mL()`  
  Returns the number of particles >0.1 µm in 0.5 L of air.

- `uint16_t getPMIn1L()`  
  Returns the number of particles >0.1 µm in 1 L of air.

- `uint16_t getPMIn2_5L()`  
  Returns the number of particles >0.1 µm in 2.5 L of air.

- `uint16_t getPMIn5L()`  
  Returns the number of particles >0.1 µm in 5 L of air.

- `uint16_t getPMIn10L()`  
  Returns the number of particles >0.1 µm in 10 L of air.

- `bool isConnected()`  
  Checks if the sensor is connected and returns `true` if connected, `false` otherwise.

#### Data Structure: `MPM10_DATA`

The `MPM10_DATA` structure is used to store sensor data and includes the following fields:

- `uint16_t pm1_0` – PM1.0 concentration in µg/m³.
- `uint16_t pm2_5` – PM2.5 concentration in µg/m³.
- `uint16_t pm10` – PM10 concentration in µg/m³.
- `bool valid` – `true` if the data is valid, `false` otherwise.

## Dependencies

- **Arduino Framework**: Required for basic functionality and hardware abstraction.
- **SensorHub Library**: A custom library used for communication with the sensor (ensure it is installed and available in your project).

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Author

Saurav Sajeev  
For questions or contributions, please contact the author or open an issue on this repository.
