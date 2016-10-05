#include "Wire.h"
#include "MPU9250.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#define I2C_ADDRESS 0x3C

I2Cdev   I2C_M;
Adafruit_BME280 bme;
SSD1306AsciiWire oled;
MPU9250 accel;

uint8_t buffer_m[6];
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
int16_t shotCount;
float Axyz[3], sumAxyz, maxAxyz;

void setup() {
  Wire.begin();
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();
  oled.setFont(Adafruit5x7);
  accel.initialize();
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("HW-Init");
  if (accel.testConnection() && bme.begin()) {
    oled.println("OK");
  } else {
    oled.println("Failed");
  }
  delay(1000);
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("Temperature");
  oled.print(bme.readTemperature());
  oled.println(" *C");
  oled.println("Pressure");
  oled.print(bme.readPressure() / 100.0F);
  oled.println(" hPa");
  oled.println("Humidity");
  oled.print(bme.readHumidity());
  oled.println(" %");
  delay(5000);
  oled.clear();
}

void loop()
{
  accel.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
  sumAxyz = sqrt(pow(Axyz[0], 2) + pow(Axyz[1], 2) + pow(Axyz[2], 2));
  if (sumAxyz > maxAxyz) {
    maxAxyz = sumAxyz;
  }
  if (sumAxyz > 2) {
    shotCount++;
    delay(50);
  }
  oled.setCursor(0, 0);
  oled.println("Shot Count:");
  oled.print(shotCount); oled.println(" shots ");
  oled.println("Acceleration: ");
  oled.print(sumAxyz); oled.println(" g ");
  oled.println("Max: ");
  oled.print(maxAxyz); oled.println(" g ");
}
