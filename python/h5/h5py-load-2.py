import os
import h5py
import numpy as np
import matplotlib.pyplot as plt

readme_file_name = '../tmp/README.md'
check_readme_file = os.path.isfile(readme_file_name)
print('Readme file exists:', check_readme_file)

model_300_file_name = '../tmp/opttrain/models/resnet56_noshort-model_300-a.h5'
check_file = os.path.isfile(model_300_file_name)
print('Model file exists:', check_file)

with h5py.File(model_300_file_name, 'r') as model_300:
  test_loss = np.array(model_300['test_loss'])

  # Find the minimum index location
  min_index = np.unravel_index(np.argmin(test_loss, axis=None), test_loss.shape)
  min_index_x = min_index[0]
  min_index_y = min_index[1]

  # Create a figure and axes
  fig, ax = plt.subplots()

  # Plot the test loss
  ax.imshow(test_loss, cmap='viridis', interpolation='nearest')

  # Add a marker at the minimum index location
  ax.plot(min_index_x, min_index_y, 'bx')

  # Set the title and labels
  ax.set_title('Minimum Index Location')
  ax.set_xlabel('X')
  ax.set_ylabel('Y')

  # Show the plot
  plt.show()

print('Loading model_300.h5 done.')