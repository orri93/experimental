import matplotlib.pyplot as plt
import math

def next_point(p, n, t):
  p[0] += (n ** 1.5 / (n + 1000.0)) * math.sin(2.0 * math.pi * t * (
    math.cos(t * 100.0) * math.sin(n * 0.05 + t * 30.0) * 10.0 + t * 10.0) + 1.8 * n * t)
  p[1] += (n ** 1.5 / (n + 1000.0)) * math.cos(2.0 * math.pi * t * (
    math.cos(t * 100.0) * math.sin(n * 0.05 + t * 30.0) * 10.0 + t * 10.0) + 1.8 * n * t)
  return p

def create_fractal(p, n, t):
  x = [p[0]]
  y = [p[1]]
  for i in range(1, n):
    p = next_point(p, n, t)
    x.append(p[0])
    y.append(p[1])
  return [x, y]

p = [0.0, 0.0]
f = create_fractal(p, 10000, 0.022499)

plt.scatter(f[0], f[1])
plt.show()
