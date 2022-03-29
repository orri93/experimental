import matplotlib.pyplot as plt
import numpy as np
import math

x = []
y1 = []
for t in np.arange(-5.0, 5.01, 0.01):
  a = np.absolute(t)
  y = math.exp(-a) * math.cos(2 * math.pi * a)
  x.append(t)
  y1.append(y)

y2 = []
for y in y1:
  y2.append(y*y)

plt.plot(x, y1)
plt.plot(x, y2)
plt.show()
