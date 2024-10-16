import socket
import struct

# Host IP and port
HOST = '172.20.10.2'  # Replace with your computer's IP address
PORT = 10000

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = (HOST, PORT)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(1)
print(f"Server started. Listening on {HOST}:{PORT}")

# Define the format string for the speed value (single int16_t)
response_format_string = '<h'  # Little-endian, int16_t

# The speed value to send
speed_value = 100  # Adjust this value as needed

while True:
    # Wait for a connection
    connection, client_address = server_socket.accept()
    try:
        print("Connection from", client_address)

        # Prepare the speed value to send
        response_data = struct.pack(response_format_string, speed_value)
        print(f"Sending speed value: {speed_value}")

        # Send the speed value to the Heltec client
        connection.sendall(response_data)
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        connection.close()
