import socket
import struct

# Host IP and port
HOST = '172.20.10.2'
PORT = 10000

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = (HOST, PORT)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen(5)
print(f"Server started. Listening on {HOST}:{PORT}")

# Define the format string for the struct with little-endian byte order
# '<' = little-endian, 'h' = int16_t, 'i' = int32_t, 'f' = float, '50s' = char[50] (byte string)
format_string = '<hif50s'
expected_packet_size = struct.calcsize(format_string)

while True:
    # Wait for a connection
    connection, client_address = server_socket.accept()
    try:
        print("Connection from", client_address)

        # Receive data from the client
        data = connection.recv(expected_packet_size)

        if len(data) == expected_packet_size:
            unpacked_data = struct.unpack(format_string, data)
            seq, distance, voltage, text = unpacked_data

            # Decode the text field, strip null termination and extra padding
            text = text.split(b'\x00', 1)[0].decode('utf-8')

            print(f"Received: seq={seq}, distance={distance}, voltage={voltage}, text={text}")

            # Prepare response
            response_data = struct.pack(format_string, seq + 1, distance + 100, voltage + 1.0, b"Response from server")
            connection.sendall(response_data)
        else:
            print(f"Received incomplete data. Expected {expected_packet_size} bytes, got {len(data)} bytes.")
    except Exception as e:
        print(f"Something went wrong: {e}")
    finally:
        connection.close()
