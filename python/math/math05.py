import matplotlib.pyplot as plt
import numpy as np
import cmath
import math

w = 1.0
k = 1.0
i = complex(0, 1)
A = complex(1, 0)
B = complex(1, 0)

t = 1
x1 = []
y1r = []
y1i = []
for x in np.arange(0.0, 10.5, 0.1):
  t1 = A * math.sin(k*x) + B * math.cos(k*x)
  t2 = cmath.exp(-i*w*t)
  f = t1 * t2
  x1.append(x)
  y1r.append(f.real)
  y1i.append(f.imag)

plt.plot(x1, y1r)
plt.plot(x1, y1i)
plt.show()
