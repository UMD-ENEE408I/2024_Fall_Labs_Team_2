import librosa
import librosa.display
import matplotlib.pyplot as plt
import numpy as np

# Load the MP3 file
def load_audio(file_path):
    # sr=None loads the file at its original sample rate
    audio, sample_rate = librosa.load(file_path, sr=None)
    return audio, sample_rate

# Generate and display the spectrogram
def plot_spectrogram(audio, sample_rate):
    # Compute the Short-Time Fourier Transform (STFT)
    stft_result = np.abs(librosa.stft(audio))

    # Convert the STFT result to decibels for better visualization
    db_spectrogram = librosa.amplitude_to_db(stft_result, ref=np.max)

    # Create a new plot
    plt.figure(figsize=(10, 6))
    
    # Display the spectrogram
    librosa.display.specshow(db_spectrogram, sr=sample_rate, x_axis='time', y_axis='log')

    # Add a color bar and labels
    plt.colorbar(format='%+2.0f dB')
    plt.title('Spectrogram')
    plt.xlabel('Time')
    plt.ylabel('Frequency (log scale)')
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Replace 'your_audio_file.mp3' with the path to your MP3 file
    audio_file_path = 'Andrew.mp3'

    # Load audio data
    audio_data, sr = load_audio(audio_file_path)

    # Plot spectrogram
    plot_spectrogram(audio_data, sr)
