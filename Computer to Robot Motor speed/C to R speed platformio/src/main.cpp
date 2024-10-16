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

// Wi-Fi network credentials
const char* ssid = "Eli's iPhone";      // Replace with your Wi-Fi SSID
const char* password = "myPassword";    // Replace with your Wi-Fi password

// Server IP and port
const char* host = "172.20.10.2";  // Replace with the server's IP address
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

  // Attach PWM channels to motor driver pins
  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Connect to the server
  if (client.connect(host, port)) {
    Serial.println("Connected to server!");
  } else {
    Serial.println("Connection to server failed.");
    return;
  }
}

// Function to set left motor speed
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

// Function to set right motor speed
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
  // Check if connected to the server
  if (client.connected()) {
    // Wait for the server to send the speed value
    int16_t speed_value = 0;

    // Wait until enough data is available
    unsigned long timeout = millis();
    while (client.available() < sizeof(int16_t)) {
      if (millis() - timeout > 5000) {  // Timeout after 5 seconds
        Serial.println("No data received from server.");
        break;
      }
    }

    if (client.available() >= sizeof(int16_t)) {
      // Read the speed value from the server
      client.readBytes((char*)&speed_value, sizeof(int16_t));

      Serial.printf("Received speed value: %d\n", speed_value);

      // Set motor speeds using the received speed value
      setLeftSpeed(speed_value);
      setRightSpeed(speed_value);
    }

    // Close the connection after receiving the speed
    client.stop();

    // Wait before attempting to reconnect
    delay(5000); // Adjust delay as necessary

    // Attempt to reconnect to the server
    if (client.connect(host, port)) {
      Serial.println("Reconnected to server!");
    } else {
      Serial.println("Reconnection failed.");
    }
  } else {
    Serial.println("Disconnected from server. Attempting to reconnect...");
    client.stop();
    delay(5000); // Wait before retrying
    if (client.connect(host, port)) {
      Serial.println("Connected to server!");
    } else {
      Serial.println("Connection to server failed.");
    }
  }
}

