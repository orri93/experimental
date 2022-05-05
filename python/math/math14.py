import matplotlib.pyplot as plt
import numpy as np
import math

d = 0.01

x = []
y1 = []
y2 = []

ly = 0.0
lf = 0.0

a = 0.509521484375
g = 0.24520874

alfa = 1.0/16.0
c1 = 1.0 / (alfa * math.sqrt(math.pi))

for i in np.arange(-0.5, 0.5 + d, d):
  f = c1 * math.exp(-(i/alfa)**2)
  y = g * (f + lf) + a * ly
  ly = y
  lf = f
  x.append(i)
  y1.append(f)
  y2.append(y)

plt.plot(x, y1)
plt.plot(x, y2)
plt.show()