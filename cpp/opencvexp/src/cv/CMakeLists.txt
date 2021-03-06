find_package(OpenCV REQUIRED core objdetect)

list(APPEND opencv_exp_cv_source
  detection.cpp)

list(APPEND opencv_exp_cv_include
  ${OpenCV_INCLUDE_DIRS}
  ${opencv_exp_include})

#list(APPEND opencv_exp_cv_libraries
#  ${OpenCV_LIBS})

# if (MSVC)
#   list(APPEND opencv_exp_cv_definitions
#     HAVE_M_PI)
# endif (MSVC)

add_library(${opencv_exp_cv_target}
  ${opencv_exp_cv_source})

if (opencv_exp_cv_definitions)
  target_compile_definitions(${opencv_exp_cv_target} PUBLIC
    ${opencv_exp_cv_definitions})
endif ()

if (opencv_exp_cv_include)
  target_include_directories(${opencv_exp_cv_target} PUBLIC
    ${opencv_exp_cv_include})
endif ()

if (opencv_exp_cv_libraries)
  target_link_libraries(${opencv_exp_cv_target} PRIVATE
    ${opencv_exp_cv_libraries})
endif ()

if (opencv_exp_cv_public_header)
  set_target_properties(${opencv_exp_cv_target}
    PROPERTIES PUBLIC_HEADER
    "${opencv_exp_cv_public_header}")
endif ()

install(TARGETS ${opencv_exp_cv_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/cv
  ARCHIVE DESTINATION lib
  RESOURCE DESTINATION lib)
