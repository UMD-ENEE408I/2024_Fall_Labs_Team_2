import numpy as np
import scipy.io.wavfile as wav
from scipy.signal import correlate

# calculate RMS of signal
def calculate_rms(audio_signal):
    square = np.square(audio_signal)
    mean = np.mean(square)
    sqrt = np.sqrt(mean)
    return sqrt

# Load files
sampling_rate_m1, signal1 = wav.read('M1.wav')
sampling_rate_m2, signal2 = wav.read('M2.wav')
sampling_rate_m3, signal3 = wav.read('M3.wav')

# sqrt fix
data1 = signal1.astype(np.float64)
data2 = signal2.astype(np.float64)
data3 = signal3.astype(np.float64)

# RMS values for each signal
rms_m1 = calculate_rms(data1)
rms_m2 = calculate_rms(data2)
rms_m3 = calculate_rms(data3)

print(f"RMS of M1: {rms_m1}")
print(f"RMS of M2: {rms_m2}")
print(f"RMS of M3: {rms_m3}")

#  cross-correlation w scipy's correlate func
cross_corr = correlate(data1, data2, mode='full')

# Find lag for max cross-correlation
lag = np.argmax(cross_corr) - (len(data1) - 1)

# Convert lag to time delay
time_delay_seconds = lag / sampling_rate_m1

print(f"Time delay in samples: {lag}")
print(f"Time delay in seconds: {time_delay_seconds} seconds")