# Exploring the date time precision in R
#
#

# Dependencies
library(lubridate)


tsstr1 <- '2024-02-12 14:01:22.1540519Z'
tsstr2 <- '2024-02-12 14:01:22.3575827Z'

ts1 <- as_datetime(tsstr1)
ts2 <- as_datetime(tsstr2)

tsdiff <- ts2 - ts1
tsdiff
