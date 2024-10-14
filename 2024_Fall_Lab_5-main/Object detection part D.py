import cv2
from ultralytics import YOLO

# Load a pre-trained YOLO model (YOLOv8 in this case)
model = YOLO('yolov8n.pt')  # You can choose different model variants like yolov8s.pt, yolov8m.pt, etc.

# Access the webcam
cap = cv2.VideoCapture(1)

# Check if the webcam is opened correctly
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

while True:
    ret, frame = cap.read()  # Capture frame-by-frame
    if not ret:
        print("Error: Failed to capture image.")
        break
    
    # Perform object detection on the frame
    results = model(frame)
    
    # Display the results
    annotated_frame = results[0].plot()  # Draw bounding boxes and labels on the frame
    cv2.imshow('YOLO Object Detection', annotated_frame)

    # Exit the loop if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and close windows
cap.release()
cv2.destroyAllWindows()
