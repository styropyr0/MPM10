#include "MPM10.h"
#include <SensorHubTemplates.h>

MPM10::MPM10(uint8_t tx, uint8_t rx) : sensorHub(MPM10_I2C_ADDR_READ)
{
    this->mode = MPM10_MODE_UART;
    this->tx = tx;
    this->rx = rx;
}

MPM10::MPM10() : sensorHub(MPM10_I2C_ADDR_READ)
{
    this->mode = MPM10_MODE_I2C;
}

MPM10_DATA MPM10::readData()
{
    MPM10_DATA data;
    data.pm1_0 = 0;
    data.pm2_5 = 0;
    data.pm10 = 0;
    data.valid = false;

    if (this->mode == MPM10_MODE_UART)
    {
        readFromUART();
        if (isValidData())
        {
            if (context == MPM10_STD_CONTEXT)
            {
                data.pm1_0 = (buffer[4] << 8) | buffer[5];
                data.pm2_5 = (buffer[6] << 8) | buffer[7];
                data.pm10 = (buffer[8] << 8) | buffer[9];
                data.valid = true;
            }
            else if (context == MPM10_ATM_CONTEXT)
            {
                data.pm1_0 = (buffer[10] << 8) | buffer[11];
                data.pm2_5 = (buffer[12] << 8) | buffer[13];
                data.pm10 = (buffer[14] << 8) | buffer[15];
                data.valid = true;
            }
        }
    }
    else
    {
        if (isConnected())
        {

            if (context == MPM10_STD_CONTEXT)
            {
                if (sensorHub.i2c_read_Xbit(MPM10_PM1_DATA_REG, &(data.pm1_0), 16) &&
                    sensorHub.i2c_read_Xbit(MPM10_PM2_5_DATA_REG, &(data.pm2_5), 16) &&
                    sensorHub.i2c_read_Xbit(MPM10_PM10_DATA_REG, &(data.pm10), 16))
                {
                    data.valid = true;
                }
                else
                {
                    data.valid = false;
                }
            }
            else if (context == MPM10_ATM_CONTEXT)
            {
                if (sensorHub.i2c_read_Xbit(MPM10_PM1_ATM_DATA_REG, &(data.pm1_0), 16) &&
                    sensorHub.i2c_read_Xbit(MPM10_PM2_5_ATM_DATA_REG, &(data.pm2_5), 16) &&
                    sensorHub.i2c_read_Xbit(MPM10_PM10_ATM_DATA_REG, &(data.pm10), 16))
                {
                    data.valid = true;
                }
                else
                {
                    data.valid = false;
                }
            }
        }
    }

    return data;
}

void MPM10::readFromUART()
{
    if (this->mode == MPM10_MODE_UART)
    {

#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_SAM_DUE) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || defined(ARDUINO_ARCH_SAMD)

        if (this->mode == MPM10_MODE_UART)
        {
#if defined(ARDUINO_ARCH_ESP32)
            Serial2.begin(MPM_10_BAUD_RATE, SERIAL_8N1, rx, tx);
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
            Serial2.begin(MPM_10_BAUD_RATE);
#elif defined(ARDUINO_ARCH_SAMD)
            Serial1.begin(MPM_10_BAUD_RATE);
#endif

            unsigned long startTime = millis();
            uint8_t index = 0;
            while (millis() - startTime < MPM10_TIMEOUT)
            {
#if defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_SAM_DUE) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
                if (Serial2.available())
                {
                    buffer[index] = Serial2.read();
#elif defined(ARDUINO_ARCH_SAMD)
                if (Serial1.available())
                {
                    buffer[index] = Serial1.read();
#endif

                    index++;
                    startTime = millis();
                    if (index >= MPM10_BUFFER_SIZE)
                        break;
                }
            }

#if defined(ARDUINO_ARCH_ESP32)
            Serial2.end();
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA)
            Serial2.end();
#elif defined(ARDUINO_ARCH_SAMD)
        Serial1.end();
#endif
        }

#else
#warning "Unsupported board! You cannot use UART with this board. Use a board that supports multiple UART ports."
#endif
    }
}

uint16_t MPM10::getPMIn300mL()
{
    return getPMCount(MPM10_PM_300ML_REG);
}

uint16_t MPM10::getPMIn500mL()
{
    return getPMCount(MPM10_PM_500ML_REG);
}

uint16_t MPM10::getPMIn1L()
{
    return getPMCount(MPM10_PM_1L_REG);
}

uint16_t MPM10::getPMIn2_5L()
{
    return getPMCount(MPM10_PM_2_5L_REG);
}

uint16_t MPM10::getPMIn5L()
{
    return getPMCount(MPM10_PM_5L_REG);
}

uint16_t MPM10::getPMIn10L()
{
    return getPMCount(MPM10_PM_10L_REG);
}

uint16_t MPM10::getPMCount(uint8_t type)
{
    uint16_t pmCount = 0;
    if (mode == MPM10_MODE_UART && isValidData())
    {
        readFromUART();
        switch (type)
        {
        case MPM10_PM_300ML_REG:
            return (buffer[16] << 8) | buffer[17];
        case MPM10_PM_500ML_REG:
            return (buffer[18] << 8) | buffer[19];
        case MPM10_PM_1L_REG:
            return (buffer[20] << 8) | buffer[21];
        case MPM10_PM_2_5L_REG:
            return (buffer[22] << 8) | buffer[23];
        case MPM10_PM_5L_REG:
            return (buffer[24] << 8) | buffer[25];
        case MPM10_PM_10L_REG:
            return (buffer[26] << 8) | buffer[27];
        default:
            return 0;
        }
    }
    else if (sensorHub.i2c_read_Xbit(type, &pmCount, 16))
        return pmCount;
    return 0;
}

void MPM10::setMeasurementContext(uint8_t context)
{
    if (context > MPM10_ATM_CONTEXT)
        return;
    this->context = context;
}

bool MPM10::isConnected()
{
    if (this->mode == MPM10_MODE_UART)
    {
        readFromUART();
        return isValidData();
    }
    else
        return sensorHub.is_sensor_connected();
}

bool MPM10::isValidData()
{
    if (buffer[0] == MPM10_FRAME_HEADER_1 && buffer[1] == MPM10_FRAME_HEADER_2)
    {
        uint16_t sum = 0;
        for (uint8_t i = 0; i < MPM10_DATA_LENGTH; i++)
            sum += buffer[i];

        uint16_t receivedChecksum = (buffer[30] << 8) | buffer[31];
        return sum == receivedChecksum;
    }

    return false;
}
