# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import math

figure, graph = plt.subplots()

a = 1 + 1j
b = 1
k = 1

c = 64
xf = 4.0
xg = 0.01
X = np.arange(0.0, xf * math.pi, xg)
t = 0.0
Y = np.exp(1j * t) * (a * np.sin(k * X) +  b * np.cos(k * X))

lr, = graph.plot(X, np.real(Y))
li, = graph.plot(X, np.imag(Y))
# lp, = graph.plot(X, np.real(Y)**2 + np.imag(Y)**2)
def animate(i):
  t = i / c
  Y = np.exp(1j * t) * (a * np.sin(k * X) +  b * np.cos(k * X))
  lr.set_ydata(np.real(Y))
  li.set_ydata(np.imag(Y))
  # lp.set_ydata(np.real(Y)**2 + np.imag(Y)**2)
  return [lr, li]

ani = animation.FuncAnimation(
  figure, animate, interval=20, blit=True, save_count=50)

plt.ylim([-2.0, 2.0])
plt.show()
