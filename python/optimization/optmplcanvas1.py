#
# Optimization Experiment 1 matplotlib canvas python class module file
#

import matplotlib
matplotlib.use('QtAgg')

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg
from matplotlib.figure import Figure

class MplCanvas(FigureCanvasQTAgg):

  def __init__(self, parent=None, width=5, height=4, dpi=100):
    self.fig = Figure(figsize=(width, height), dpi=dpi)
    self.axes = self.fig.add_subplot(111)
    super(MplCanvas, self).__init__(self.fig)

if __name__ == "__main__":
  text = input("Yell something at a mountain: ")
  print(text)