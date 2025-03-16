#include <MPM10.h>

MPM10 sensor(17, 16); // Constructor for UART mode (TX pin 17, RX pin 16)

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for Serial Monitor to open

  if (sensor.isConnected())
  {
    Serial.println("MPM10 sensor connected successfully!");
  }
  else
  {
    Serial.println("Failed to connect to MPM10 sensor!");
    while (1)
      ; // Halt execution
  }

  // Set measurement context to atmospheric
  sensor.setMeasurementContext(MPM10_ATM_CONTEXT);
}

void loop()
{
  MPM10_DATA data = sensor.readData();

  if (data.valid)
  {
    Serial.print("PM1.0: ");
    Serial.print(data.pm1_0);
    Serial.println(" µg/m³");
    Serial.print("PM2.5: ");
    Serial.print(data.pm2_5);
    Serial.println(" µg/m³");
    Serial.print("PM10: ");
    Serial.print(data.pm10);
    Serial.println(" µg/m³");

    // Read particle counts in various air volumes
    Serial.print("Particles in 300 mL: ");
    Serial.println(sensor.getPMIn300mL());
    Serial.print("Particles in 1 L: ");
    Serial.println(sensor.getPMIn1L());
    Serial.print("Particles in 10 L: ");
    Serial.println(sensor.getPMIn10L());
  }
  else
  {
    Serial.println("Invalid data received from sensor!");
  }

  delay(5000); // Wait 5 seconds before the next reading
}