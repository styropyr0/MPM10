#include <MPM10.h>

// Create an instance of the MPM10 class for I2C mode
MPM10 sensor;

void setup()
{
    // Initialize the Serial Monitor for debugging
    Serial.begin(9600);
    while (!Serial)
        ; // Wait for Serial Monitor to open

    // Check if the sensor is connected
    if (sensor.isConnected())
    {
        Serial.println("MPM10 sensor connected successfully via I2C!");
    }
    else
    {
        Serial.println("Failed to connect to MPM10 sensor via I2C!");
        while (1)
            ; // Halt execution if the sensor is not connected
    }

    // Optional: Set the measurement context (default is standard)
    // Use MPM10_ATM_CONTEXT for atmospheric measurements if needed
    sensor.setMeasurementContext(MPM10_STD_CONTEXT);
}

void loop()
{
    // Read sensor data
    MPM10_DATA data = sensor.readData();

    // Check if the data is valid
    if (data.valid)
    {
        // Print PM concentrations
        Serial.println("=== Particulate Matter Concentrations ===");
        Serial.print("PM1.0: ");
        Serial.print(data.pm1_0);
        Serial.println(" µg/m³");
        Serial.print("PM2.5: ");
        Serial.print(data.pm2_5);
        Serial.println(" µg/m³");
        Serial.print("PM10:  ");
        Serial.print(data.pm10);
        Serial.println(" µg/m³");

        // Print particle counts in various air volumes
        Serial.println("\n=== Particle Counts in Air Volumes ===");
        Serial.print("Particles >0.1 µm in 300 mL: ");
        Serial.println(sensor.getPMIn300mL());
        Serial.print("Particles >0.1 µm in 0.5 L:  ");
        Serial.println(sensor.getPMIn500mL());
        Serial.print("Particles >0.1 µm in 1 L:    ");
        Serial.println(sensor.getPMIn1L());
        Serial.print("Particles >0.1 µm in 2.5 L:  ");
        Serial.println(sensor.getPMIn2_5L());
        Serial.print("Particles >0.1 µm in 5 L:    ");
        Serial.println(sensor.getPMIn5L());
        Serial.print("Particles >0.1 µm in 10 L:   ");
        Serial.println(sensor.getPMIn10L());
    }
    else
    {
        Serial.println("Invalid data received from MPM10 sensor!");
    }

    // Wait 5 seconds before the next reading
    Serial.println("\nWaiting for next reading...");
    delay(5000);
}