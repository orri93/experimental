# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

# Initialise the subplot function using number of rows and columns
figure, graph = plt.subplots(4, 1)

# Get the angles from 0 to 2 pie (360 degree) in narray object
X = np.arange(0, math.pi, 0.01)

Y1 = np.exp(-X) * np.cos(X * np.pi)
Y2 = np.exp(-X) * np.sin(2.333 * X * np.pi)
Y3 = np.exp(-X) * np.sin(4.666 * X * np.pi)
Y4 = Y1 + Y2 + Y3

graph[0].set_title("F1")
graph[0].set_ylim([-1, 1])
line1, = graph[0].plot(X, Y1)

graph[1].set_title("F2")
graph[1].set_ylim([-1, 1])
line2, = graph[1].plot(X, Y2)

graph[2].set_title("F3")
graph[2].set_ylim([-1, 1])
line3, = graph[2].plot(X, Y3)

graph[3].set_title("F4")
graph[3].set_ylim([-4, 4])
line4, = graph[3].plot(X, Y4)

def animate(i):
  c = 16
  Y1 = np.exp(-X) * np.sin(i / c + X * np.pi)
  Y2 = np.exp(-X) * np.sin(i / c + 2.333 * X * np.pi)
  Y3 = np.exp(-X) * np.sin(i / c + 4.666 * X * np.pi)
  Y4 = Y1 + Y2 + Y3
  line1.set_ydata(Y1)
  line2.set_ydata(Y2)
  line3.set_ydata(Y3)
  line4.set_ydata(Y4)
  return [line1, line2, line3, line4]

ani = animation.FuncAnimation(
  figure, animate, interval=4, blit=True, save_count=50)

# Combine all the operations and display
plt.show()
