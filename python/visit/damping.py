import numpy as np
import math

f = open("damping.csv", "wt")
f.write("x y z value\n")

for t1 in np.arange(-2.0, 2.01, 0.01):
  for t2 in np.arange(-2.0, 2.01, 0.01):
    ax = np.absolute(t1)
    ay = np.absolute(t2)
    fx = math.exp(-ax) * math.cos(2 * math.pi * ax)
    fy = math.exp(-ay) * math.cos(2 * math.pi * ay)
    z = fx * fy
    f.write("%g %g %g %g\n" % (t1, t2, z * z, z))
f.close()