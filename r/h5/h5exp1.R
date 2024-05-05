# Install rhdf5 package (only need to run if not already installed)
#install.packages("BiocManager")
#BiocManager::install("rhdf5")

# Load the R HDF5 Library
library("rhdf5")

fid <- H5Fopen("../tmp/opttrain/models/resnet56_noshort-model_300-a.h5")

xdir <- H5Dopen(fid, "test_loss")
