import json

fs = open('trajectory.json')
fo = open("trajectory.csv", "wt")
fo.write("depth,inclination,azimuth\n")

data = json.load(fs)

for t in data['TrajData']:
  fo.write("%g,%g,%g\n" % (t[0],t[1],t[2]))

fs.close()
fo.close()