import cv2
import numpy as np
import socket
import struct

# Video capture settings
cap = cv2.VideoCapture(1)
cap.set(3, 160)  # Width
cap.set(4, 120)  # Height

# TCP/IP communication settings
HOST = '172.20.10.2'  # Your computer's IP address
PORT = 10000          # Same port as the Heltec robot expects

# Define the format string for motor speeds
# '<' = little-endian, 'h' = int16_t
motor_speeds_format = '<2h'  # Left and right motor speeds

# Thresholds for detecting sharp turns
area_threshold = 500  # Adjust based on your observations
error_threshold = 160 / 4  # Assuming frame width is 160 pixels

# Speed settings
turn_speed = 100  # Speed during a sharp turn (adjust as needed)
base_speed = 75   # Base motor speed
Kp = 0.1          # Proportional gain; adjust as needed

# Create a TCP/IP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)
print(f"Server started. Listening on port {PORT}")

# Wait for a connection
connection, client_address = server_socket.accept()
print(f"Connection from {client_address}")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        # Preprocessing
        # Convert to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Apply Gaussian blur to reduce noise
        blur = cv2.GaussianBlur(gray, (5, 5), 0)
        # Apply threshold
        _, mask = cv2.threshold(blur, 200, 255, cv2.THRESH_BINARY)

        # Use a Region of Interest (ROI)
        # Define ROI coordinates (modify as needed)
        height, width = mask.shape
        roi_top = int(height * 0.5)
        roi_bottom = height
        roi = mask[roi_top:roi_bottom, :]

        # Find contours in the ROI
        contours, _ = cv2.findContours(roi, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # Filter out small contours
        min_contour_area = 10000  # Adjust based on your observations
        contours = [c for c in contours if cv2.contourArea(c) > min_contour_area]

        if contours:
            # Find the largest contour
            c = max(contours, key=cv2.contourArea)
            area = cv2.contourArea(c)
            M = cv2.moments(c)

            if M["m00"] != 0:
                # Calculate centroid in the ROI
                cx = int(M['m10'] / M['m00'])
                cy = int(M['m01'] / M['m00']) + roi_top  # Adjust cy based on ROI position

                # Draw centroid on the frame
                cv2.circle(frame, (cx, cy), 5, (255, 0, 0), -1)

                # Calculate error
                error = cx - width / 2

                # Determine if a sharp turn is needed
                if area < area_threshold or abs(error) > error_threshold:
                    # Sharp turn detected
                    if error > 0:
                        # Turn Right
                        left_speed = turn_speed
                        right_speed = -turn_speed
                        print("Performing sharp right turn")
                    else:
                        # Turn Left
                        left_speed = -turn_speed
                        right_speed = turn_speed
                        print("Performing sharp left turn")
                else:
                    # Regular line following
                    turn = Kp * error

                    left_speed = int(base_speed + turn)
                    right_speed = int(base_speed - turn)

                    print(f"Left Speed: {left_speed}, Right Speed: {right_speed}")

                # Constrain speeds
                left_speed = max(-255, min(255, -left_speed))
                right_speed = max(-255, min(255, -right_speed))

                # Pack and send motor speeds
                motor_data = struct.pack(motor_speeds_format, left_speed, right_speed)
                connection.sendall(motor_data)
            else:
                print("Line detected but moment is zero")
                # Stop the robot
                left_speed = 0
                right_speed = 0
                motor_data = struct.pack(motor_speeds_format, left_speed, right_speed)
                connection.sendall(motor_data)
        else:
            # No contours found; stop the robot
            print("Line not detected")
            left_speed = 0
            right_speed = 0
            motor_data = struct.pack(motor_speeds_format, left_speed, right_speed)
            connection.sendall(motor_data)

        # Display the frame and ROI
        cv2.imshow("Frame", frame)
        cv2.imshow("Mask", mask)
        cv2.imshow("ROI", roi)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            # Stop the robot before exiting
            left_speed = 0
            right_speed = 0
            motor_data = struct.pack(motor_speeds_format, left_speed, right_speed)
            connection.sendall(motor_data)
            break

except Exception as e:
    print(f"An error occurred: {e}")

finally:
    connection.close()
    cap.release()
    cv2.destroyAllWindows()
