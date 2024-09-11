import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator

# will create an array of values starting from 0 up to (but not including) 5π, with a step size of 0.1.
x = np.arange(0, 2*np.pi, 0.1)
# will compute the sine of each value in the array x
y = np.sin(x)

# Plotting Sine Graph
plt.plot(x, y, color='green')
plt.xlabel('Time (s)')
plt.ylabel('Magnitude')
plt.show()


# 3-D plot
fig, ax = plt.subplots(subplot_kw={"projection": "3d"})

# function
X = np.arange(-5, 5, 0.25)
Y = np.arange(-5, 5, 0.25)
X, Y = np.meshgrid(X, Y)
R = np.sqrt(X**2 + Y**2)
Z = np.sin(R)

# Plot function
surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=False)

# color bar
fig.colorbar(surf, shrink=0.5, aspect=5)

plt.show()