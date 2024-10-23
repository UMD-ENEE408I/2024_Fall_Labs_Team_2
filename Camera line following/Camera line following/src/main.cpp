#include <WiFi.h>

// Motor Pins
const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

// PWM Channels
const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

const int freq = 5000;
const int resolution = 8;

// Wi-Fi Credentials
const char* ssid = "Eli's iPhone";      // Your Wi-Fi SSID
const char* password = "myPassword";    // Your Wi-Fi password

// Server IP and port
const char* host = "172.20.10.2";  // Replace with your computer's IP
const uint16_t port = 10000;

// Create a client
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Setup PWM channels
  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  // Attach PWM channels to motor pins
  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Connect to the server
  if (client.connect(host, port)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
  }
}

void setLeftSpeed(int speed) {
  int pwm = constrain(abs(speed), 0, 255);
  if (speed > 0) {
    // Forward
    ledcWrite(M1_IN_1_CHANNEL, pwm);
    ledcWrite(M1_IN_2_CHANNEL, 0);
  } else if (speed < 0) {
    // Reverse
    ledcWrite(M1_IN_1_CHANNEL, 0);
    ledcWrite(M1_IN_2_CHANNEL, pwm);
  } else {
    // Stop
    ledcWrite(M1_IN_1_CHANNEL, 0);
    ledcWrite(M1_IN_2_CHANNEL, 0);
  }
}

void setRightSpeed(int speed) {
  int pwm = constrain(abs(speed), 0, 255);
  if (speed > 0) {
    // Forward
    ledcWrite(M2_IN_1_CHANNEL, pwm);
    ledcWrite(M2_IN_2_CHANNEL, 0);
  } else if (speed < 0) {
    // Reverse
    ledcWrite(M2_IN_1_CHANNEL, 0);
    ledcWrite(M2_IN_2_CHANNEL, pwm);
  } else {
    // Stop
    ledcWrite(M2_IN_1_CHANNEL, 0);
    ledcWrite(M2_IN_2_CHANNEL, 0);
  }
}

void loop() {
  if (client.connected()) {
    // Check if data is available
    if (client.available() >= sizeof(int16_t) * 2) {
      int16_t left_speed = 0;
      int16_t right_speed = 0;

      // Receive motor speeds
      client.readBytes((char*)&left_speed, sizeof(int16_t));
      client.readBytes((char*)&right_speed, sizeof(int16_t));

      Serial.printf("Received Speeds - Left: %d, Right: %d\n", left_speed, right_speed);

      // Set motor speeds
      setLeftSpeed(left_speed);
      setRightSpeed(right_speed);
    }
  } else {
    Serial.println("Disconnected from server. Reconnecting...");
    client.stop();
    delay(1000);
    if (client.connect(host, port)) {
      Serial.println("Reconnected to server");
    } else {
      Serial.println("Reconnection failed");
    }
  }

  delay(10);  // Small delay to prevent overwhelming the loop
}


