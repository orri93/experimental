import logic.moving_average as ma

window_size = 3
moving_average = ma.MovingAverage(window_size)
values = [1, 2, 3, 4, 5]
for value in values:
  moving_average.add(value)
  print(f'Added {value}, average: {moving_average.average()}')

