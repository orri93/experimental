import matplotlib.pyplot as plt
import numpy as np
import cmath
import math

x = []
y = []
for i in np.arange(0.0, 10.5, 0.5):
  f = cmath.exp(complex(0, i))
  x.append(i)
  y.append(f.real**2 + f.imag**2)

plt.scatter(x, y)
plt.show()
