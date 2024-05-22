#
# Optimization Experiment 1 optimization algorithm 1 python class module file
#

import math
import random

import numpy as np

State = ['Idle', 'Step', 'Evaluate']
RandomOptions = ['General', 'Normal Distribution']

class Point:
  def __init__(self, x_init, y_init):
    self.x = x_init
    self.y = y_init

class Algorithm:
  def __init__(self):
    self.state = State[0]
    self.landscape = None
    self.current_point = None
    self.next_point = None
    self.landscape_max_x = None
    self.landscape_max_y = None
    self.min_step_ratio = None
    self.max_step_ratio = None

    self.random_option = 0

    self.normal_mu = 0
    self.normal_sigma = 0.1

    self.step_count = 0

  def seed(self, seed):
    if (seed > 0):
      random.seed(seed)
      np.random.seed(seed)
    else:
      random.seed()
      np.random.seed()

  def set_landscape(self, landscape):
    self.landscape = landscape
    self.landscape_max_x = self.landscape.shape[0] - 1
    self.landscape_max_y = self.landscape.shape[1] - 1

  def set_min_step_ratio(self, min_step_ratio):
    self.min_step_ratio = min_step_ratio

  def set_max_step_ratio(self, max_step_ratio):
    self.max_step_ratio = max_step_ratio

  def set_random_option(self, random_option):
    self.random_option = random_option
  
  def set_normal_mu(self, normal_mu):
    self.normal_mu = normal_mu

  def set_normal_sigma(self, normal_sigma):
    self.normal_sigma = normal_sigma

  def first(self):
    self.step_count = 0
    self.state = State[1]
    current_x = random.randint(0, self.landscape_max_x)
    current_y = random.randint(0, self.landscape_max_y)
    self.current_point = Point(current_x, current_y)
    return self.current_point
  
  def next(self):
    self.step_count = self.step_count + 1
    if (self.current_point.x == 0 and self.current_point.y == 0):
      theta = random.uniform(3 * math.pi / 2 , 2 * math.pi)
    elif (self.current_point.x == 0 and self.current_point.y == self.landscape_max_y):
      theta = random.uniform(0, math.pi / 2)
    elif (self.current_point.x == self.landscape_max_x and self.current_point.y == 0):
      theta = random.uniform(math.pi, 3 * math.pi / 2)
    elif (self.current_point.x == self.landscape_max_x and self.current_point.y == self.landscape_max_y):
      theta = random.uniform(math.pi / 2, math.pi)
    elif (self.current_point.x == 0):
      theta = random.uniform(3 * math.pi / 2, 5 * math.pi)
    elif (self.current_point.y == 0):
      theta = random.uniform(0, math.pi)
    elif (self.current_point.x == self.landscape_max_x):
      theta = random.uniform(math.pi / 2, 3 * math.pi / 2)
    elif (self.current_point.y == self.landscape_max_y):
      theta = random.uniform(math.pi, 2 * math.pi)
    else:
      theta = random.uniform(0, 2 * math.pi)
    hypotenuse = math.sqrt(self.landscape_max_x * self.landscape_max_x + self.landscape_max_y + self.landscape_max_y)
    min_step = self.min_step_ratio * hypotenuse
    max_step = self.max_step_ratio * hypotenuse
    if (self.random_option == 0):
      extent = random.uniform(min_step, max_step)
    else:
      step = max_step - min_step
      extent = min_step + step * np.random.normal(self.normal_mu, self.normal_sigma)
    new_x = self.current_point.x + int(extent * math.cos(theta))
    new_y = self.current_point.y + int(extent * math.sin(theta))
    if (new_x < 0):
      new_x = 0
    elif (new_x > self.landscape_max_x):
      new_x = self.landscape_max_x
    if (new_y < 0):
      new_y = 0
    elif (new_y > self.landscape_max_y):
      new_y = self.landscape_max_y
    self.next_point = Point(new_x, new_y)
    self.state = State[2]
    return self.next_point
  
  def evaluate(self):
    current_value = self.landscape[self.current_point.x, self.current_point.y]
    next_value = self.landscape[self.next_point.x, self.next_point.y]
    is_better = next_value < current_value
    if (is_better):
      self.current_point = self.next_point
    self.state = State[1]
    return is_better
  
  def get_current_minimum(self):
    return self.landscape[self.current_point.x, self.current_point.y]

  def get_step_count(self):
    return self.step_count
