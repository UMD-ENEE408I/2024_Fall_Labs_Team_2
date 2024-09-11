#include <Arduino.h>
#include <Encoder.h>

const unsigned int M1_ENC_A = 39;
const unsigned int M1_ENC_B = 38;
const unsigned int M2_ENC_A = 37;
const unsigned int M2_ENC_B = 36;

const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const unsigned int PWM_VALUE = 255; // Max PWM given 8 bit resolution

const int freq = 5000;
const int resolution = 8;

void setup() {
  // Stop the right motor by setting pin 14 low
  // this pin floats high or is pulled
  // high during the bootloader phase for some reason
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  delay(100);

  Serial.begin(115200);
  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  pinMode(M1_I_SENSE, INPUT);
  pinMode(M2_I_SENSE, INPUT);

  pinMode(M2_IN_2, OUTPUT);
  pinMode(M1_IN_2, OUTPUT);
  pinMode(M2_IN_1, OUTPUT);

  pinMode(M1_IN_1, OUTPUT);

}

void loop() {
  // Create the encoder objects after the motor has
  // stopped, else some sort exception is triggered
  Encoder enc1(M1_ENC_A, M1_ENC_B);
  Encoder enc2(M2_ENC_A, M2_ENC_B);

  long enc1_value = 0; 
  long enc2_value = 0; 
  digitalWrite(M1_IN_2, HIGH);
  digitalWrite(M1_IN_1, HIGH);
  while (enc1_value <= 360) {
    enc1_value = enc1.read();
    enc2_value = enc2.read();
    Serial.print("left: ");
    Serial.println(enc1_value);
    Serial.print("\t\tright: ");
    Serial.println(enc2_value);
    Serial.print("\n");
  }
  digitalWrite(M1_IN_2, LOW);
  digitalWrite(M1_IN_1, LOW);
  delay(2000);

  digitalWrite(M1_IN_1, HIGH);
  digitalWrite(M1_IN_2, HIGH);
  while (enc1_value >= 0) {
    enc1_value = enc1.read();
    enc2_value = enc2.read();
    Serial.print("left: ");
    Serial.println(enc1_value);
    Serial.print("\t\tright: ");
    Serial.println(enc2_value);
    Serial.print("\n");
  }
  digitalWrite(M1_IN_1, LOW);
  digitalWrite(M1_IN_2, LOW);
  delay(2000);
}