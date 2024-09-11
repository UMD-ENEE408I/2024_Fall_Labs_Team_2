import numpy as np
import scipy.io.wavfile as wav
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

sampling_rate, signal = wav.read('Cafe_with_noise.wav')

# Time axis for the plot (number of samples / sampling rate gives time in seconds)
time_axis = np.linspace(0, len(signal) / sampling_rate, num=len(signal))

# fft of signal
signal_fft = np.fft.fft(signal)
frequencies = np.fft.fftfreq(len(signal), d=1/sampling_rate)

# Low-pass filter
def butter_lowpass_filter(data, cutoff_freq, sampling_rate, order=5):
    nyquist_freq = 0.5 * sampling_rate
    normal_cutoff = cutoff_freq / nyquist_freq
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    filtered_signal = filtfilt(b, a, data)
    return filtered_signal

# Apply filter w cutoff frequency for human voice range
cutoff_freq = 3400
filtered_signal = butter_lowpass_filter(signal, cutoff_freq, sampling_rate)

plt.figure(figsize=(12, 10))

# original audio signal plot
plt.subplot(3, 1, 1)
plt.plot(time_axis, signal, color='b')
plt.title('Original Audio Signal (Time Domain)')
plt.xlabel('Time [seconds]')
plt.ylabel('Amplitude')

# frequency spectrum of original signal
plt.subplot(3, 1, 2)
plt.plot(frequencies[:len(frequencies)//2], np.abs(signal_fft[:len(signal)//2]), color='r')
plt.title('Frequency Spectrum of the Original Signal')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Magnitude')

# filtered signal plot
plt.subplot(3, 1, 3)
plt.plot(time_axis, filtered_signal, color='g')
plt.title('Filtered Audio Signal (Time Domain)')
plt.xlabel('Time [seconds]')
plt.ylabel('Amplitude')

plt.tight_layout()
plt.show()
