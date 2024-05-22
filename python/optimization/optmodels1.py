#
# Optimization Experiment 1 Models python module file
#

import os

import h5py
import numpy as np

model_path = '../tmp/opttrain/models/'
model_names = ['train_acc', 'train_loss', 'test_acc', 'test_loss', 'loss_vals']

def get_model_files():
  if not os.path.exists(model_path):
    raise FileNotFoundError(f"The model path '{model_path}' does not exist.")
  if not os.path.isdir(model_path):
    raise NotADirectoryError(f"The model path '{model_path}' is not a directory.")
  model_extensions = ['.h5', '.hdf5']
  model_files = [fn for fn in os.listdir(model_path)
    if any(fn.endswith(ext) for ext in model_extensions)]
  return model_files

def load_model(model_file):
  model_file_name = os.path.join(model_path, model_file)
  if not os.path.exists(model_file_name):
    raise FileNotFoundError(f"The model file '{model_file_name}' does not exist.")
  if not os.path.isfile(model_file_name):
    raise IsADirectoryError(f"The model file '{model_file_name}' is a directory.")
  return h5py.File(model_file_name, 'r')

def get_model_keys(model):
  return [ key for key in model.keys() if any(key.startswith(prefix) for prefix in model_names) ]

def get_model_data(model, key):
  return np.array(model[key])