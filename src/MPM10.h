#ifndef MPM10_H
#define MPM10_H

#include <Arduino.h>
#include <SensorHub.h>
#include <SensorHubTemplates.h>
#include "MPM10DEFS.h"

#ifdef __cplusplus

/**
 * @struct MPM10_DATA
 * @brief Structure to store MPM10 sensor data.
 *
 * The MPM10_DATA structure is used to store the particulate matter
 * sensor data read from the MPM10 sensor.
 *
 * @var pm1_0 PM1.0 concentration in µg/m³.
 * @var pm2_5 PM2.5 concentration in µg/m³.
 * @var pm10 PM10 concentration in µg/m³.
 * @var valid True if the data is valid, false otherwise.
 */
typedef struct
{
    uint16_t pm1_0;
    uint16_t pm2_5;
    uint16_t pm10;
    bool valid;
} MPM10_DATA;

/**
 * @class MPM10
 * @brief Driver for the MPM10 particulate matter sensor.
 *
 * The MPM10 class provides functionality to interact with the MPM10
 * particulate matter sensor. It includes methods to read sensor data
 * and check if the sensor is connected.
 *
 * @note This class supports both UART and I2C communication modes.
 *
 * @author Saurav Sajeev
 */
class MPM10
{
public:
    /**
     * @brief Constructor for the MPM10 class.
     * By default, I2C mode is enabled. If you wish to use UART, use the other constructor.
     */
    MPM10();

    /**
     * @brief Constructor for the MPM10 class.
     * @param  tx UART transmit pin.
     * @param  rx UART receive pin.
     */
    MPM10(uint8_t tx, uint8_t rx);

    /**
     * @brief Read sensor data from the MPM10 sensor.
     * @return MPM10_DATA Sensor data.
     */
    MPM10_DATA readData();

    /**
     * @brief Set the measurement context for the MPM10 sensor. By default, it uses the standard context. Change to use the atmospheric context.
     * @param context Measurement context (Use MPM10_STD_CONTEXT or MPM10_ATM_CONTEXT).
     */
    void setMeasurementContext(uint8_t context);

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 300 mL of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 300 mL of air.
     */
    uint16_t getPMIn300mL();

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 0.5 L of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 0.5 L of air.
     */
    uint16_t getPMIn500mL();

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 1 L of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 1 L of air.
     */
    uint16_t getPMIn1L();

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 2.5 L of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 2.5 L of air.
     */
    uint16_t getPMIn2_5L();

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 5 L of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 5 L of air.
     */
    uint16_t getPMIn5L();

    /**
     * @brief Get the number of particles of size greater than 0.1 µm in 10 L of air.
     * @return uint16_t The number of particles of size greater than 0.1 µm in 10 L of air.
     */
    uint16_t getPMIn10L();

    /**
     * @brief Check if the MPM10 sensor is connected.
     * @return bool True if the sensor is connected, false otherwise.
     */
    bool isConnected();

private:
    uint8_t mode, context = MPM10_STD_CONTEXT;
    uint8_t buffer[MPM10_BUFFER_SIZE];
    uint8_t tx, rx;
    SensorHub sensorHub;

    bool isValidData();
    void readFromUART();
    uint16_t getPMCount(uint8_t type);
};

#endif
#endif