add_subdirectory(fastnoise)
add_subdirectory(gosoutpp)

if (GOS_NN_EXPERIMENTAL)
  add_subdirectory(nn)
endif ()

if (GOS_QT_EXPERIMENTAL)
  add_subdirectory(qt)
endif ()
