import math
import numpy as np

f = open("math01.csv", "wt")
f.write("x y z value\n")

for ix in np.arange(-2, 2, 0.1):
  for iy in np.arange(-2, 2, 0.1):
    for iz in np.arange(-2, 2, 0.1):
      if ix != 0.0 and iy != 0.0 and iz != 0.0:
        r = math.sqrt(ix*ix + iy*iy + iz*iz)
        theta = math.atan(math.sqrt(ix*ix + iy*iy) / iz)
        phi = math.atan(iy / ix)
        v = math.sin(theta)
        f.write("%g %g %g %g\n" % (ix,iy,iz,v))

f.close()