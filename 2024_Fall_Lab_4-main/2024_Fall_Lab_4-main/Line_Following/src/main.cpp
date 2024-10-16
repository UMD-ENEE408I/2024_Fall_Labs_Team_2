#include <Arduino.h>
#include <Adafruit_MCP3008.h>
#include <Encoder.h>

// ADC (line sensor)
Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

uint8_t lineArray[13]; 

// Motors
const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

const unsigned int PWM_MAX = 255;
const int freq = 5000;
const int resolution = 8;

// PID constants
float Kp = 2.0;
float Ki = 0.01;
float Kd = 0.01;

// PID variables
unsigned long lastTime = 0;
float errorSum = 0;
float lastError = 0;
unsigned long sampleTime = 1; // in milliseconds

int baseSpeed = 75; // Base speed for motors

void setup() {
  Serial.begin(115200);

  // Setup PWM channels
  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  // Attach PWM channels to motor driver pins
  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  // Initialize ADC
  adc1.begin(ADC_1_CS);
  adc2.begin(ADC_2_CS);
}

void readADC() {
  for (int i = 0; i < 8; i++) {
    adc1_buf[i] = adc1.readADC(i);
    adc2_buf[i] = adc2.readADC(i);
  }
}

// Converts ADC readings to binary array lineArray[]
void digitalConvert() {
  int threshold = 700;
  for (int i = 0; i < 7; i++) {
    lineArray[2 * i] = adc1_buf[i] > threshold ? 0 : 1;
    if (i < 6) {
      lineArray[2 * i + 1] = adc2_buf[i] > threshold ? 0 : 1;
    }
  }
}

// Calculate robot's position on the line (0-12)
float getPosition() {
  float weightedSum = 0;
  float totalWeight = 0;

  // Calculate the weighted sum of sensor positions
  for (int i = 0; i < 13; i++) {
    if (lineArray[i] == 1) {
      weightedSum += i;      // Accumulate position values where sensor reads the line
      totalWeight += 1;      // Count active sensors
    }
  }

  if (totalWeight == 0) {
    return 6.0;  // Assume centered if no line detected
  }

  return weightedSum / totalWeight;  // Return weighted average position
}

// Set motor speed functions
void setLeftSpeed(int speed) {
  int pwm = constrain(abs(speed), 0, 255);
  ledcWrite(M1_IN_1_CHANNEL, speed > 0 ? pwm : 0);
  ledcWrite(M1_IN_2_CHANNEL, speed < 0 ? pwm : 0);
}

void setRightSpeed(int speed) {
  int pwm = constrain(abs(speed), 0, 255);
  ledcWrite(M2_IN_1_CHANNEL, speed > 0 ? pwm : 0);
  ledcWrite(M2_IN_2_CHANNEL, speed < 0 ? pwm : 0);
}

void loop() {
  // Read and process line sensor data
  readADC();
  digitalConvert();
  float position = getPosition();  // Calculate position of the robot on the line

  // Define the PID error
  float error = position - 6.0;  // Desired position is 6 (centered)
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;

  // PID calculations
  errorSum += error * deltaTime;
  float dError = (error - lastError) / deltaTime;
  float output = Kp * error + Ki * errorSum + Kd * dError;

  // Update motor speeds
  int leftSpeed = baseSpeed - (int)output;
  int rightSpeed = baseSpeed + (int)output;

  // Set motor speeds
  setLeftSpeed(-leftSpeed);
  setRightSpeed(-rightSpeed);

  // Update variables
  lastError = error;
  lastTime = currentTime;

  // Debug output to Serial Monitor
  Serial.print("Position: ");
  Serial.print(position);
  Serial.print("\tError: ");
  Serial.print(error);
  Serial.print("\tLeft Speed: ");
  Serial.print(leftSpeed);
  Serial.print("\tRight Speed: ");
  Serial.println(rightSpeed);

  delay(50);  // Adjust delay as necessary
}
