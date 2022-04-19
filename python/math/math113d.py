# importing libraries
from matplotlib import projections
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

figure = plt.figure()
graph = figure.add_subplot(projection='3d')

# t = 3.0
t = 0.0
c = 64
xf = 2.0
xg = 0.01
twopi = 2.0 * math.pi
X = np.arange(0.0, xf * math.pi, xg)
Y = np.exp(-(X - t)**2) * (np.cos(twopi * (X - t)) +  np.sin(twopi * (X - t)) * 1j)

l = graph.plot(X, np.real(Y), np.imag(Y))

def animate(i):
  t = i / c
  Y = np.exp(-(X - t)**2) * (np.cos(twopi * (X - t)) +  np.sin(twopi * (X - t)) * 1j)
  d = [np.real(Y), np.imag(Y)]
  l.set_data(d[:2])
  return l

ani = animation.FuncAnimation(
  figure, animate, interval=20, blit=True, save_count=50)

plt.show()
