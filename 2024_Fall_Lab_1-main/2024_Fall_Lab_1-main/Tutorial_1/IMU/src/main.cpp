#include <Arduino.h>
// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

float angle_z = 0;  // Store the accumulated rotation angle
unsigned long previous_time = 0;

void setup(void) {
  // Stop the right motor by setting pin 14 low
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);
  
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  pinMode(ADC_1_CS, OUTPUT);
  pinMode(ADC_2_CS, OUTPUT);

  digitalWrite(ADC_1_CS, HIGH); 
  digitalWrite(ADC_2_CS, HIGH); 

  // Try to initialize MPU6050!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("");
  delay(100);

  previous_time = millis();  // Record the initial time
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the sensor values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.print(" m/s^2 \t");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.print(" rad/s \t");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  // Calculate elapsed time
  unsigned long current_time = millis();
  float time_diff = (current_time - previous_time) / 1000.0;  // Convert to seconds
  previous_time = current_time;

  // Integrate the angular velocity (gyroscope Z-axis) to get angle in degrees
  angle_z += g.gyro.z * time_diff * (180 / PI);  // rad/s to degrees

  Serial.print("Accumulated Z Angle: ");
  Serial.println(angle_z);

  // Rotate motor 90 degrees
  if (angle_z < 90) {
    analogWrite(14, 100); 
  } else if (angle_z < 180) {
    analogWrite(14, 100);  // Keep moving until 180 degrees
  } else {
    analogWrite(14, 0);  // Stop motor after 180 degrees
  }

  delay(100);
}
