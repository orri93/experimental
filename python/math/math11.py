# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

figure, graph = plt.subplots()

c = 64
xf = 4.0
xg = 0.01
twopi = 2.0 * math.pi
X = np.arange(0.0, xf * math.pi, xg)
Y = np.exp(-X**2) * (np.cos(twopi * X) +  np.sin(twopi * X) * 1j)

lr, = graph.plot(X, np.real(Y))
li, = graph.plot(X, np.imag(Y))
lp, = graph.plot(X, np.real(Y)**2 + np.imag(Y)**2)
def animate(i):
  t = i / c
  Y = np.exp(-(X - t)**2) * (np.cos(twopi * (X - t)) +  np.sin(twopi * (X - t)) * 1j)
  lr.set_ydata(np.real(Y))
  li.set_ydata(np.imag(Y))
  lp.set_ydata(np.real(Y)**2 + np.imag(Y)**2)
  return [lr, li, lp]

ani = animation.FuncAnimation(
  figure, animate, interval=20, blit=True, save_count=50)

plt.ylim([-1.2, 1.2])
plt.show()

