# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

# Initialise the subplot function using number of rows and columns
figure, graph = plt.subplots(2, 2)

# Get the angles from 0 to 2 pie (360 degree) in narray object
X = np.arange(0.0, 2.0 * math.pi, 0.01)

ratio1 = 0.5
ratio2 = 1.0
ratio3 = 1.5
ratio4 = 2.0

Y1 = (1.0 * np.sin(ratio1 * X))**2 + (0.0 * np.sin(ratio1 * X))**2
Y2 = (1.0 * np.sin(ratio2 * X))**2 + (0.0 * np.sin(ratio2 * X))**2
Y3 = (1.0 * np.sin(ratio3 * X))**2 + (0.0 * np.sin(ratio3 * X))**2
Y4 = (1.0 * np.sin(ratio4 * X))**2 + (0.0 * np.sin(ratio4 * X))**2

graph[0, 0].set_title("F1")
graph[0, 0].set_ylim([-1, 1])
line1, = graph[0, 0].plot(X, Y1)

graph[0, 1].set_title("F2")
graph[0, 1].set_ylim([-1, 1])
line2, = graph[0, 1].plot(X, Y2)

graph[1, 0].set_title("F3")
graph[1, 0].set_ylim([-1, 1])
line3, = graph[1, 0].plot(X, Y3)

graph[1, 1].set_title("F3")
graph[1, 1].set_ylim([-1, 1])
line4, = graph[1, 1].plot(X, Y4)

def animate(i):
  c = 16
  sinratio = np.sin(i / c)
  cosratio = np.cos(i / c)
  Y1 = (sinratio * np.sin(ratio1 * X))**2 + (cosratio * np.sin(ratio1 * X))**2
  Y2 = (sinratio * np.sin(ratio2 * X))**2 + (cosratio * np.sin(ratio2 * X))**2
  Y3 = (sinratio * np.sin(ratio3 * X))**2 + (cosratio * np.sin(ratio3 * X))**2
  Y4 = (sinratio * np.sin(ratio4 * X))**2 + (cosratio * np.sin(ratio4 * X))**2
  line1.set_ydata(Y1)
  line2.set_ydata(Y2)
  line3.set_ydata(Y3)
  line4.set_ydata(Y4)
  return [line1, line2, line3, line4]

ani = animation.FuncAnimation(
  figure, animate, interval=4, blit=True, save_count=50)

# Combine all the operations and display
plt.show()
