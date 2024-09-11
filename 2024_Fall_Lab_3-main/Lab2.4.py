import numpy as np
from scipy import linalg
from scipy.optimize import minimize
from numpy.fft import fft,fftfreq
import matplotlib.pyplot as plt

# Coefficient matrix A
A = np.array([[3, 1], [1, 2]])

# Constants vector b
b = np.array([9, 8])

# Solve the system of equations
x = linalg.solve(A, b)

print("Solution vector x:", x)

def func(x):
    return x**2 + 2*x
    
# minimization starting point    
x0 = 0

min = minimize(func,x0)
print(min.fun)



# Define the function
def func(x):
    return np.sin(100 * np.pi * x) + 1/2 * np.sin(160 * np.pi * x)

# Create the x values
sampling_rate = 600
T = 1.0/sampling_rate
x = np.linspace(0.0, 1.0, sampling_rate)

# Evaluate the function at each point in x
y = func(x)

# Perform the Fourier transform
ft = fft(y)

# Taking positive freqs
xf = fftfreq(sampling_rate, T)[:sampling_rate//2]

# Plot Freq Response
plt.plot(xf, 2.0/sampling_rate * np.abs(ft[:sampling_rate//2]))
plt.title('Frequency Response')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.grid()
plt.show()