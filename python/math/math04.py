import matplotlib.pyplot as plt
import numpy as np
import math

r = 1.0
x1 = []
x2 = []
y1 = []
y2 = []
for t in np.arange(0.0, 2.0 * math.pi, 0.01):
  ar = r * math.cos(t)
  br = r * math.sin(t)
  x1.append(ar)
  y1.append(br)
  u = r + math.sin(4 * t) * 0.1
  au = u * math.cos(t)
  bu = u * math.sin(t)
  x2.append(au)
  y2.append(bu)

plt.plot(x1, y1)
plt.plot(x2, y2)
plt.show()
