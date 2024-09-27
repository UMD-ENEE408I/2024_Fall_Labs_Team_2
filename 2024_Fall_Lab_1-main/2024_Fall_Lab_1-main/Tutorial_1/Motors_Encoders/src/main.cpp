#include <Arduino.h>
#include <Encoder.h>

// Encoder pins
const unsigned int M1_ENC_A = 39;
const unsigned int M1_ENC_B = 38;
const unsigned int M2_ENC_A = 37;
const unsigned int M2_ENC_B = 36;

// Motor driver pins
const unsigned int M1_IN_1 = 13;  // Direction control pin 1 for motor 1
const unsigned int M1_IN_2 = 12;  // Direction control pin 2 for motor 1
const unsigned int M2_IN_1 = 25;  // Direction control pin 1 for motor 2
const unsigned int M2_IN_2 = 14;  // Direction control pin 2 for motor 2

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 2;
const unsigned int M2_IN_2_CHANNEL = 3;

// PWM settings
const int freq = 5000;       // PWM frequency
const int resolution = 8;    // PWM resolution (8 bits)

// PID constants
float Kp = 2;  // Start with a moderate Kp value
float Ki = 0.01;  // Initially set to zero
float Kd = 0.01;  // Initially set to zero

// PID variables
unsigned long lastTime = 0;
float errorSum = 0;
float lastError = 0;
unsigned long sampleTime = 1; // in milliseconds

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
}

void setLeftSpeed(int speed) {
  int pwm = abs(speed);
  pwm = constrain(pwm, 0, 255);

  if (speed > 0) {
    // Forward
    ledcWrite(M1_IN_1_CHANNEL, pwm);
    ledcWrite(M1_IN_2_CHANNEL, 0);
  } else if (speed < 0) {
    // Backward
    ledcWrite(M1_IN_1_CHANNEL, 0);
    ledcWrite(M1_IN_2_CHANNEL, pwm);
  } else {
    // Stop
    ledcWrite(M1_IN_1_CHANNEL, 0);
    ledcWrite(M1_IN_2_CHANNEL, 0);
  }
}

void setRightSpeed(int speed) {
  int pwm = abs(speed);
  pwm = constrain(pwm, 0, 255);

  if (speed > 0) {
    // Forward
    ledcWrite(M2_IN_1_CHANNEL, pwm);
    ledcWrite(M2_IN_2_CHANNEL, 0);
  } else if (speed < 0) {
    // Backward
    ledcWrite(M2_IN_1_CHANNEL, 0);
    ledcWrite(M2_IN_2_CHANNEL, pwm);
  } else {
    // Stop
    ledcWrite(M2_IN_1_CHANNEL, 0);
    ledcWrite(M2_IN_2_CHANNEL, 0);
  }
}

void loop() {
   // Initialize encoders as static variables
  static Encoder enc1(M1_ENC_A, M1_ENC_B);
  static Encoder enc2(M2_ENC_A, M2_ENC_B);

  long enc1_value = 0;
  long enc2_value = 0;

  int baseSpeed = 100; // Adjust base speed as necessary

  // Compute maximum allowed output
  float maxOutput = 50; // Adjust max output to prevent large corrections

  // Reset PID variables
  lastTime = millis();
  errorSum = 0;
  lastError = 0;

  // Reset encoder counts
  enc1.write(0);
  enc2.write(0);

  // PID control loop runs indefinitely
  while (true) {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= sampleTime) {
        // Read encoder values
        enc1_value = enc1.read();
        enc2_value = -enc2.read(); // Invert enc2 readings

        // Compute error
        float error = (float)(enc2_value - enc1_value);

        // Compute time difference in seconds
        float deltaTime = (currentTime - lastTime) / 1000.0;

        // Update integral term with windup protection
        errorSum += error * deltaTime;
        errorSum = constrain(errorSum, -1000, 1000); // Adjust limits as needed

        // Compute derivative term
        float dError = (error - lastError) / deltaTime;

        // Compute PID output
        float output = Kp * error + Ki * errorSum + Kd * dError;

        // Limit output to prevent speeds from going out of bounds
        output = constrain(output, -maxOutput, maxOutput);

        // Adjust motor speeds in opposite directions
        int leftSpeed = baseSpeed - (int)(output);
        int rightSpeed = baseSpeed + (int)(output);

        // Constrain motor speeds to valid range
        leftSpeed = constrain(leftSpeed, 0, 255);
        rightSpeed = constrain(rightSpeed, 0, 255);

        // Set motor speeds
        setLeftSpeed(leftSpeed);
        setRightSpeed(rightSpeed);

        // Update last error and last time
        lastError = error;
        lastTime = currentTime;

        // Print debug information to Serial Monitor
        Serial.print("Enc1: ");
        Serial.print(enc1_value);
        Serial.print("\tEnc2: ");
        Serial.print(enc2_value);
        Serial.print("\tError: ");
        Serial.print(error);
        Serial.print("\tOutput: ");
        Serial.println(output);
        Serial.print("Left Speed: ");
        Serial.print(leftSpeed);
        Serial.print("\tRight Speed: ");
        Serial.println(rightSpeed);
    }
  }
}
