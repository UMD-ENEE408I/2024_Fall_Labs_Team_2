#include <WiFi.h>

// Motors
const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

struct __attribute__((packed)) Data {
  int16_t seq;       // Sequence number to track packet order
  int32_t distance;  // Example distance data
  float voltage;     // Example voltage data
  char text[50];     // Example text message
};


// WiFi network credentials
const char* ssid = "Eli's iPhone";
const char* password = "myPassword";

// Server IP and port
const char* host = "172.20.10.4";  // Replace with the IP address of server
const uint16_t port = 10000;

// Create a client
WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Connect to the server
  if (client.connect(host, port)) {
    Serial.println("Connected to server!");
  } else {
    Serial.println("Connection to server failed.");
    return;
  }
}

void loop() {

    // Prepare data packet
    Data data;
    data.seq = 1;
    data.distance = 1000;
    data.voltage = 3.7f;
    strncpy(data.text, "Hello from ESP32!", sizeof(data.text) - 1);
    data.text[sizeof(data.text) - 1] = '\0'; // Ensure null termination
    Serial.printf("seq %d distance %ld voltage %f text %s\n", data.seq, data.distance, data.voltage, data.text);

    // Check if connected to the server
    if (client.connected()) {
      // Read server's response (if any)
      while (client.available()) {
        Data response;
        client.readBytes((char*)&response, sizeof(response)); // Read data from the server and unpack it into the response struct
        Serial.printf("seq %d distaace %ld voltage %f text %s\n", (int)response.seq, (long)response.distance, response.voltage, response.text);
      }

      // Send data to the server
      client.write((uint8_t*)&data, sizeof(data));

      // Increment sequence number for the next packet and add a delay between messages
      data.seq++;
      delay(5000); // Send data every 5 seconds
    } else {
      Serial.println("Disconnected from server.");
      client.stop();
    }
}