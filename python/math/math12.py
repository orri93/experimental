# importing libraries
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import numpy as np
import cmath
import math

figure, graph = plt.subplots()

c = 128
k0 = 16.0
xf = 6.0
xg = 0.01
twopi = 2.0 * math.pi
pic1 = math.sqrt(math.sqrt(2.0/math.pi)) # the fourth root of 2/π
X = np.arange(0.0, xf * math.pi, xg)

def fn1(t):
  return cmath.sqrt(1.0 + 2.0 * 1j * t)

def fn2(t):
  return pic1 / fn1(t)

def fn3(t):
  return 1.0 / (1.0 + 4.0 * t * t)

Y = fn2(0.0) * np.exp(X**2) * np.exp(1j * k0 * X) * (np.cos(twopi * X) +  np.sin(twopi * X) * 1j)

lr, = graph.plot(X, np.real(Y))
li, = graph.plot(X, np.imag(Y))
lp, = graph.plot(X, np.real(Y)**2 + np.imag(Y)**2)
def animate(i):
  t = i / c
  Y = fn2(t) * np.exp(-fn3(t) * ((X - k0 * t)**2)) * np.exp(1j * fn3(t) * ( (k0 + 2.0 * t * X) * X - t * k0 * k0 / 2.0 ) ) * (np.cos(twopi * (X - t)) +  np.sin(twopi * (X - t)) * 1j)
  lr.set_ydata(np.real(Y))
  li.set_ydata(np.imag(Y))
  lp.set_ydata(np.real(Y)**2 + np.imag(Y)**2)
  return [lr, li, lp]

ani = animation.FuncAnimation(
  figure, animate, interval=20, blit=True, save_count=50)

plt.ylim([-1.2, 1.2])
plt.show()
