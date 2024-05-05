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
  # Print all root level object names (aka keys) 
  # these can be group or dataset names 
  print("Keys: %s" % model_300.keys())
  # get first object name/key; may or may NOT be a group
  a_group_key = list(model_300.keys())[0]

  # get the object type for a_group_key: usually group or dataset
  print(type(model_300[a_group_key]))

  # If a_group_key is a group name,
  # this gets the object names in the group and returns as a list
  data = list(model_300[a_group_key])

  # preferred methods to get dataset values:
  ds_obj = model_300[a_group_key]      # returns as a h5py dataset object
  #ds_arr = model_300[a_group_key][()]  # returns as a numpy array

print('Loading model_300.h5 done.')