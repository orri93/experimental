#
# Optimization Experiment 1 optimization chart 1 python class module file
#

import numpy as np

import optmplcanvas1
import optalg1

class Chart:
  def __init__(self):
    self.sc = None
    self.date = None
    self.minimum_index = None
    self.current_point = None
    self.next_point = None

  def handover_canvas(self, sc):
    self.sc = sc

  def set_data(self, date):
    self.date = date

  def set_minimum_index(self, index):
    self.minimum_index = index

  def set_current_point(self, point):
    self.current_point = point

  def set_next_point(self, point):
    self.next_point = point

  def clear(self):
    self.current_point = None
    self.next_point = None

  def plot(self):
    self.sc.axes.clear()
    self.sc.axes.imshow(self.date, cmap='viridis', interpolation='nearest')
    # Add a marker at the minimum index location
    self.sc.axes.plot(self.minimum_index[0], self.minimum_index[1], 'bx')
    if (self.current_point is not None):
      self.sc.axes.plot(self.current_point.x, self.current_point.y, 'ro')
    if (self.next_point is not None):
      self.sc.axes.plot(self.next_point.x, self.next_point.y, 'go')
    self.sc.draw()