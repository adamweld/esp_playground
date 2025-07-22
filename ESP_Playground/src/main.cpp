

#include <Arduino.h>
#include <Wire.h>
#include <ADXL345.h>

ADXL345 accel;

void setup() {
  Serial.begin(115200);
  delay(10000);
  
  printf("ADXL345 Accelerometer Test with I2Cdevlib\n");
  printf("Initializing sensor...\n");
  
  Wire.begin(13, 15);
  accel.initialize();
  
  // Enable measurement mode
  accel.setMeasureEnabled(true);
  
  // Set range to +/-16g for better sensitivity
  accel.setRange(ADXL345_RANGE_16G);
  
  // Set data rate to 100Hz
  accel.setRate(ADXL345_RATE_100);
  
  // Enable full resolution mode
  accel.setFullResolution(true);
  
  printf("Testing device connection...");
  if (accel.testConnection()) {
    printf(" SUCCESS!\n");
  } else {
    printf(" FAILED!\n");
    printf("Check wiring and I2C address.\n");
    while(1);
  }
  
  printf("ADXL345 sensor initialized successfully!\n");
  
  printf("---- Sensor Configuration ----\n");
  printf("Device ID: 0x%02X\n", accel.getDeviceID());
  printf("Data Rate: %d\n", accel.getRate());
  printf("Range: +/-%dg\n", accel.getRange());
  printf("Full Resolution: %s\n", accel.getFullResolution() ? "Enabled" : "Disabled");
  printf("Measurement Mode: %s\n", accel.getMeasureEnabled() ? "Enabled" : "Disabled");
  printf("------------------------------\n");
  printf("\n");
  printf("Starting accelerometer readings...\n");
  printf("X\t\tY\t\tZ\n");
}

void loop() {
  int16_t ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);
  
  // Debug: Check if we're getting any data
  printf("Raw Acceleration - X: %d, Y: %d, Z: %d\n", ax, ay, az);
  
  // Additional debug: Check register values directly
  // printf("Power Control: 0x%02X\n", accel.getPowerControl());
  // printf("Data Format: 0x%02X\n", accel.getDataFormat());
  
  // Convert to g-force (scale factor depends on range setting)
  // For +/-16g range: 1g = 256 LSB, so multiply by (1/256) = 0.00390625
  float x_g = ax * 0.00390625;
  float y_g = ay * 0.00390625;
  float z_g = az * 0.00390625;
  
  printf("Acceleration (g) - X: %.3f, Y: %.3f, Z: %.3f\n", x_g, y_g, z_g);
  
  float x_ms2 = x_g * 9.80665;
  float y_ms2 = y_g * 9.80665;
  float z_ms2 = z_g * 9.80665;
  
  printf("Acceleration (m/s²) - X: %.2f, Y: %.2f, Z: %.2f\n", x_ms2, y_ms2, z_ms2);
  
  float magnitude = sqrt(pow(x_ms2, 2) + pow(y_ms2, 2) + pow(z_ms2, 2));
  printf("Magnitude: %.2f m/s²\n", magnitude);
  
  printf("Connection Status: %s\n", accel.testConnection() ? "OK" : "LOST");
  printf("----------------------------------------\n");
  
  delay(1000);
}
