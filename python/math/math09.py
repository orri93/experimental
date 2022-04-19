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

Y1R = 1.0 * np.sin(ratio1 * X)
Y1I = 0.0 * np.sin(ratio1 * X)
Y1A = Y1R**2 + Y1I**2
Y2R = 1.0 * np.sin(ratio2 * X)
Y2I = 0.0 * np.sin(ratio2 * X)
Y2A = Y2R**2 + Y2I**2
Y3R = 1.0 * np.sin(ratio3 * X)
Y3I = 0.0 * np.sin(ratio3 * X)
Y3A = Y3R**2 + Y3I**2
Y4R = 1.0 * np.sin(ratio4 * X)
Y4I = 0.0 * np.sin(ratio4 * X)
Y4A = Y4R**2 + Y4I**2

graph[0, 0].set_title("F1")
graph[0, 0].set_ylim([-1, 1])
line1r, = graph[0, 0].plot(X, Y1R)
line1i, = graph[0, 0].plot(X, Y1I)
line1a, = graph[0, 0].plot(X, Y1A)

graph[0, 1].set_title("F2")
graph[0, 1].set_ylim([-1, 1])
line2r, = graph[0, 1].plot(X, Y2R)
line2i, = graph[0, 1].plot(X, Y2I)
line2a, = graph[0, 1].plot(X, Y2A)

graph[1, 0].set_title("F3")
graph[1, 0].set_ylim([-1, 1])
line3r, = graph[1, 0].plot(X, Y3R)
line3i, = graph[1, 0].plot(X, Y3I)
line3a, = graph[1, 0].plot(X, Y3A)

graph[1, 1].set_title("F3")
graph[1, 1].set_ylim([-1, 1])
line4r, = graph[1, 1].plot(X, Y4R)
line4i, = graph[1, 1].plot(X, Y4I)
line4a, = graph[1, 1].plot(X, Y4A)

def animate(i):
  c = 16
  sinratio = np.sin(i / c)
  cosratio = np.cos(i / c)
  Y1R = sinratio * np.sin(ratio1 * X)
  Y1I = cosratio * np.sin(ratio1 * X)
  Y2R = sinratio * np.sin(ratio2 * X)
  Y2I = cosratio * np.sin(ratio2 * X)
  Y3R = sinratio * np.sin(ratio3 * X)
  Y3I = cosratio * np.sin(ratio3 * X)
  Y4R = sinratio * np.sin(ratio4 * X)
  Y4I = cosratio * np.sin(ratio4 * X)
  line1r.set_ydata(Y1R)
  line1i.set_ydata(Y1I)
  line2r.set_ydata(Y2R)
  line2i.set_ydata(Y2I)
  line3r.set_ydata(Y3R)
  line3i.set_ydata(Y3I)
  line4r.set_ydata(Y4R)
  line4i.set_ydata(Y4I)
  return [line1r,line1i,line1a, line2r,line2i,line2a, line3r,line3i,line3a, line4r,line4i,line4a]

anim = animation.FuncAnimation(
  figure, animate, frames=100, interval=4, blit=True, save_count=50)

# Combine all the operations and display

anim.save('math09.gif', writer='imagemagick', fps=60)

plt.show()
