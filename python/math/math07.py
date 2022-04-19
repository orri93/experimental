# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

# Initialise the subplot function using number of rows and columns
figure, graph = plt.subplots(4, 1)

# Get the angles from 0 to 2 pie (360 degree) in narray object
X = np.arange(0, 16 * math.pi, 0.05)

Y1 = np.sin(X)
Y2 = np.sin(X * 2.333)
Y3 = np.sin(X * 4.666)
Y4 = Y1 + Y2 + Y3

graph[0].set_title("F1")
line1, = graph[0].plot(X, Y1)

graph[1].set_title("F2")
line2, = graph[1].plot(X, Y2)

graph[2].set_title("F3")
line3, = graph[2].plot(X, Y3)

graph[3].set_title("F4")
line4, = graph[3].plot(X, Y4)

def animate(i):
  Y1 = np.sin(i / 50 + X)
  Y2 = np.sin(i / 50 + X * 2.333)
  Y3 = np.sin(i / 50 + X * 4.666)
  Y4 = Y1 + Y2 + Y3
  line1.set_ydata(Y1)
  line2.set_ydata(Y2)
  line3.set_ydata(Y3)
  line4.set_ydata(Y4)
  return [line1, line2, line3, line4]

ani = animation.FuncAnimation(
  figure, animate, interval=5, blit=True, save_count=50)

# Combine all the operations and display
plt.show()
