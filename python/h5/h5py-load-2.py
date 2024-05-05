import os
import h5py
import numpy as np

readme_file_name = '../tmp/README.md'
check_readme_file = os.path.isfile(readme_file_name)
print('Readme file exists:', check_readme_file)

model_300_file_name = '../tmp/opttrain/densenet121/densenet121_sgd_lr=0.1_bs=64_wd=0.0005/model_300.t7_weights_xignore=biasbn_xnorm=filter_yignore=biasbn_ynorm=filter.h5'
check_file = os.path.isfile(model_300_file_name)
print('Model file exists:', check_file)

with h5py.File(model_300_file_name, 'r') as model_300:
  xdirection = model_300['xdirection']
  ydirection = model_300['ydirection']
  xlist = list(xdirection)
  ylist = list(ydirection)

print('Loading model_300.h5 done.')