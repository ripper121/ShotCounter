#include "Wire.h"
#include "MPU9250.h"
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;


// class default I2C address is 0x68
MPU9250 accel;
I2Cdev   I2C_M;

uint8_t buffer_m[6];

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
int16_t shotCount;

float Axyz[3], sumAxyz, maxAxyz;
void setup() {
  Wire.begin();
  Serial.begin(250000);
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.set400kHz();
  oled.setFont(Adafruit5x7);
  accel.initialize();
  oled.clear();
  oled.setCursor(0, 0);
  oled.println("MPU6050");
  if (accel.testConnection()) {
    oled.println("Connected");
  } else {
    oled.println("Failed");
  }
  delay(1000);
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
  if(sumAxyz>2){
    shotCount++;
    delay(50);
  }
  oled.setCursor(0, 0);
  oled.println("Shot Count:");
  oled.print(shotCount);oled.println(" shots ");
  oled.println("Sum: ");
  oled.print(sumAxyz);oled.print(" g ");
  oled.print(maxAxyz);oled.println(" g ");
  Serial.println(sumAxyz);
}
