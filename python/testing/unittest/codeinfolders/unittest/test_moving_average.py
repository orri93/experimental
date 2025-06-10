import unittest

import logic.moving_average as ma

class TestMovingAverage(unittest.TestCase):
  def test_moving_average(self):
    window_size = 3
    moving_average = ma.MovingAverage(window_size)
    values = [1, 2, 3, 4, 5]
    averages = [None, None, 2, 3, 4]
    for value, average in zip(values, averages):
      moving_average.add(value)
      self.assertEqual(moving_average.average(), average)

if __name__ == '__main__':
  unittest.main()