cmake_minimum_required(VERSION 3.8.2)

project(OpenCvExperiments
  VERSION 1.0.0
  DESCRIPTION "GOS OpenCV Experiments"
  LANGUAGES CXX)

# Build Options
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Tidy Options
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

#  Options

# GOS Test Options
option(USE_GTEST_DISCOVER
  "Use CMake Google Test discovery for unit tests" OFF)
option(USE_GTEST_PACKAGE
  "Use CMake GTest Package for unit tests" OFF)

set(BUILD_NUMBER "0" CACHE STRING "The build number")

set(opencv_exp_source_include "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(opencv_exp_binary_include "${CMAKE_CURRENT_BINARY_DIR}/include")

list(APPEND opencv_exp_include
  ${opencv_exp_source_include}
  ${opencv_exp_binary_include})

set(opencv_exp_cv_target cv)
list(APPEND opencv_exp_cv_public_header
  "${CMAKE_CURRENT_SOURCE_DIR}/include/cv/detection.h")

add_subdirectory(src)

message(STATUS "")
message(STATUS "${PROJECT_DESCRIPTION} ${PROJECT_VERSION} BUILD SUMMARY")
message(STATUS "  Install prefix            : ${CMAKE_INSTALL_PREFIX}")

message(STATUS "  CMAKE_GENERATOR           : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_SOURCE_DIR          : ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_CURRENT_SOURCE_DIR  : ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "  BUILD_SHARED_LIBS         : ${BUILD_SHARED_LIBS}")

message(STATUS "  C   Compiler ID           : ${CMAKE_C_COMPILER_ID}")
message(STATUS "  C   Compiler Version      : ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  C   Compiler flags        : ${CMAKE_C_FLAGS}")
message(STATUS "  C++ Compiler ID           : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  C++ Compiler Version      : ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  C++ Compiler flags        : ${CMAKE_CXX_FLAGS}")

if (MSVC_VERSION)
message(STATUS "  MSVC Version              : ${MSVC_VERSION}")
endif (MSVC_VERSION)

message(STATUS "  OpenCV Include            : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "  OpenCV Libraries          : ${OpenCV_LIBS}")

if (BUILD_TESTS)
  message(STATUS "Building Tests")
  if (MSVC_VERSION)
    message(STATUS "  GTest Force Shared CRT    : ${gtest_force_shared_crt}")
  endif ()

  enable_testing()
# set(unit_testing_xml_output_dir )
  if(USE_GTEST_PACKAGE)
    find_package(GTest REQUIRED)
  else ()
    if (MSVC)
      #if (FORCE_SHARED_CRT)
        set(gtest_force_shared_crt ON CACHE BOOL "Use Shared CRT")
      #endif ()
    endif (MSVC)
    if (EXPB_INSTRUCT_GOOGLE_TEST_PATH)
      set(EXPB_GOOGLE_TEST_PATH
        "${EXPB_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(EXPB_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")
#     add_subdirectory(extern/googletest)
    endif ()
    set(efd_experiment_c_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(efd_experiment_c_unit_testing_gmock_include_dir
      "${EXPB_GOOGLE_TEST_PATH}/googlemock/include")
    set(efd_experiment_c_unit_testing_gtest_include_dir
      "${EXPB_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND efd_experiment_c_gtest_include
      ${efd_experiment_c_unit_testing_gmock_include_dir}
      ${efd_experiment_c_unit_testing_gtest_include_dir})
    list(APPEND efd_experiment_c_gtest_libraries
      gtest
      gmock
      gmock_main)
  endif ()
  add_subdirectory(tests)
endif (BUILD_TESTS)
