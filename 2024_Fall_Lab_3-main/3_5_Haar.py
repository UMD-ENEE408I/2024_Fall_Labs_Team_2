import cv2

# Load the image (assuming it's saved on disk)
src = cv2.imread('sample face.jpg')

# Convert the image to grayscale
gray = cv2.cvtColor(src, cv2.COLOR_RGBA2GRAY)

# Load the pre-trained Haar cascade classifiers for face and eyes detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

# Detect faces in the grayscale image
faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=3, minSize=(0, 0))

# Loop over detected faces
for (x, y, w, h) in faces:
    # Draw a rectangle around each face
    cv2.rectangle(src, (x, y), (x + w, y + h), (255, 0, 0), 2)

    # Get the region of interest (ROI) for the face in the grayscale image and color image
    roi_gray = gray[y:y + h, x:x + w]
    roi_color = src[y:y + h, x:x + w]

    # Detect eyes within the face region
    eyes = eye_cascade.detectMultiScale(roi_gray)
    for (ex, ey, ew, eh) in eyes:
        # Draw a rectangle around each eye
        cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (0, 0, 255), 2)

# Display the output image with detected faces and eyes
cv2.imshow('canvasOutput', src)

# Wait for a key press and close the window
cv2.waitKey(0)
cv2.destroyAllWindows()
