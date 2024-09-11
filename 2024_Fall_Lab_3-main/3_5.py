import cv2 as cv

# Read the image
img = cv.imread('sample.jpg')

# grayscale the image
img_gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

# Display grayscaled image in a window
cv.imshow('Grayscaled Image', img_gray)

# Wait for a key press and close the window
cv.waitKey(0)
cv.destroyAllWindows()


# edge detection
dst = cv.Canny(img, 50, 100, apertureSize=3, L2gradient=False)

# Display edge detection img in a window
cv.imshow('Edge Detection', dst)

# Wait for a key press and close the window
cv.waitKey(0)
cv.destroyAllWindows()