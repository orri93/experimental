cmake_minimum_required(VERSION 3.8.2)

project(GosExperimental
  VERSION 1.0.0
  DESCRIPTION "Geirmundur Orri Sigurdsson Experimental"
  LANGUAGES C CXX)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

option(GOS_NN_EXPERIMENTAL "GOS Experimental for Neural Network" OFF)
option(GOS_SPB_EXPERIMENTAL "GOS Experimental for Sparkplug B" OFF)
option(GOS_SDL_WEB_EXPERIMENTAL "GOS Experimental for SDL Web" OFF)
option(GOS_SDL_EXPERIMENTAL "GOS Experimental for SDL" OFF)
option(GOS_SDL2_EXPERIMENTAL "GOS Experimental for SDL2" OFF)
option(GOS_QT_EXPERIMENTAL "GOS Experimental for Qt" OFF)

option(GOS_USE_GTEST_DISCOVER "Use CMake Google Test descovery for unit tests" OFF)
option(GOS_USE_GTEST_PACKAGE "Use CMake GTest Package for unit tests" OFF)

set(GOS_BUILD_NUMBER "0" CACHE STRING "The build number")

set(TIDY_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy check")
set(TIDY_FIX_ARGUMENTS "-list-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix")
set(TIDY_FIX_ERRORS_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix errors")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/experimental/version.h.in"
  "include/gos/experimental/version.h"
  @ONLY)

list(APPEND gos_experimental_include
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

string(TIMESTAMP GOS_CURRENT_YEAR "%Y" UTC)
set(GOS_COMPANYNAME "Geirmundur Orri Sigurdsson")
set(GOS_LEGALCOPYRIGHT "Copyright (C) ${GOS_CURRENT_YEAR} ${GOS_COMPANYNAME}")

list(APPEND gos_experimental_library_public_headers
  "${CMAKE_CURRENT_BINARY_DIR}/include/gos/experimental/version.h")

set(gos_experimental_library_target libgosexperimental)

set(gos_c_library_target libgos)
list(APPEND gos_c_library_public_header
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/line.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/types.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/color.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/noise.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/perlin.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/signal.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/mapping.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/geometry.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/interpolate.h")

set(gos_c_drilling_library_target libgosdrilling)
list(APPEND gos_c_drilling_library_public_header
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/drilling/density.h")

if(CLANG_TIDY)
  find_program(CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable")
  if(CLANG_TIDY_EXE)
    if(CLANG_TIDY_FIX_ERRORS)
      set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE}" "${TIDY_FIX_ERRORS_ARGUMENTS}-fix-errors")
      message(STATUS "Using clang-tidy with fix")
      message(STATUS "  ${CLANG_TIDY_EXE} ${TIDY_FIX_ERRORS_ARGUMENTS}-fix-errors")
    elseif(CLANG_TIDY_FIX)
      set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "${TIDY_FIX_ARGUMENTS}-fix")
      message(STATUS "Using clang-tidy with fix")
      message(STATUS "  ${CLANG_TIDY_EXE} ${TIDY_FIX_ARGUMENTS}-fix")
    else()
      set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE}" "${TIDY_ARGUMENTS}")
      message(STATUS "Using clang-tidy")
      message(STATUS "  ${CLANG_TIDY_EXE} ${TIDY_ARGUMENTS}")
    endif()
  endif()
endif()

# ##############################################################################
# External dependency

# GOSC
set(gos_ext_gosc_dir "${CMAKE_SOURCE_DIR}/extern/gosc")
add_subdirectory(extern/gosc)

if (GOS_NN_EXPERIMENTAL)
  set(gos_nn_experimental_opennn_dir "${CMAKE_SOURCE_DIR}/extern/opennn")
  set(gos_nn_experimental_opennn_library opennn)
  list(APPEND gos_nn_experimental_opennn_include
    "${gos_nn_experimental_opennn_dir}/opennn"
    "${gos_nn_experimental_opennn_dir}")
  add_subdirectory(extern/opennn)
endif ()

if (GOS_SPB_EXPERIMENTAL)
  set(gos_spb_experimental_mosquitto_dir "${CMAKE_SOURCE_DIR}/extern/mosquitto")
  set(gos_spb_experimental_mosquitto_lib_dir "${gos_spb_experimental_mosquitto_dir}/lib")
  set(gos_spb_experimental_mosquitto_library libmosquitto_static)
  add_subdirectory(extern/mosquitto)
  set(gos_spb_experimental_nanopb_dir "${CMAKE_SOURCE_DIR}/extern/nanopb")
  set(gos_spb_experimental_nanopb_library protobuf-nanopb-static)
  set(gos_spb_experimental_nanopb_generator_py "${gos_spb_experimental_nanopb_dir}/generator/nanopb_generator.py")
  add_subdirectory(extern/nanopb)

  set(gos_spb_experimental_tahu_dir "${CMAKE_SOURCE_DIR}/extern/tahu")
  set(gos_spb_experimental_c_tahuspb_binary_dir "${CMAKE_BINARY_DIR}/c/tahuspb")

  set(gos_spb_experimental_tahu_c_library tahuspbc)
  list(APPEND gos_spb_experimental_c_include
    ${gos_spb_experimental_c_tahuspb_binary_dir}
    ${gos_spb_experimental_mosquitto_lib_dir}
    ${gos_spb_experimental_nanopb_dir})

  set(gos_spb_experimental_tahu_cpp_library tahuspbcpp)

endif ()

set(gos_fast_noise_library_directory
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/FastNoise")
set(gos_fast_noise_library_target libfastnoise)
list(APPEND gos_fast_noise_library_public_header
  "${gos_fast_noise_library_directory}/FastNoise.h")

#set(Boost_USE_MULTITHREADED        ON)
#set(Boost_USE_STATIC_RUNTIME      OFF)
#if (BUILD_SHARED_LIBS)
#  set(Boost_USE_STATIC_LIBS       OFF)    
#else ()
#  set(Boost_USE_STATIC_LIBS        ON)
#endif ()

#list(APPEND needed_boost_package
#  program_options
#  filesystem
#  date_time
#  system
#  regex)
#list(APPEND needed_boost_libraries
#  Boost::program_options
#  Boost::filesystem
#  Boost::date_time
#  Boost::regex)

#if (GOS_USE_BOOST_LOG)
#  list(APPEND needed_boost_package
#    log_setup
#    log)
#  list(APPEND needed_boost_libraries
#    Boost::log_setup
#    Boost::log)
#endif ()

#find_package(Boost 1.70.0 COMPONENTS REQUIRED
#  ${needed_boost_package})

#list(APPEND nov_wellbore_connect_modules_include
#  ${Boost_INCLUDE_DIRS})

#list(APPEND gos_experimental_include
#  ${needed_boost_libraries})
 
add_subdirectory(c)
add_subdirectory(cpp)

if (BUILD_TESTS)
  enable_testing()
# set(gos_unit_testing_xml_output_dir )
  if(GOS_USE_GTEST_PACKAGE)
    find_package(GTest REQUIRED)
  else ()
    if (MSVC)
      #if (GOS_FORCE_SHARED_CRT)
        set(gtest_force_shared_crt ON CACHE BOOL "Use Shared CRT")
      #endif ()
    endif (MSVC)
    if (GOS_EXPERIMENTAL_INSTRUCT_GOOGLE_TEST_PATH)
      set(GOS_EXPERIMENTAL_GOOGLE_TEST_PATH
        "${GOS_EXPERIMENTAL_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(GOS_EXPERIMENTAL_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/googletest")
      add_subdirectory(extern/googletest)
    endif ()
    set(gos_experimental_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(gos_experimental_unit_testing_gmock_include_dir
      "${GOS_EXPERIMENTAL_GOOGLE_TEST_PATH}/googlemock/include")
    set(gos_experimental_unit_testing_gtest_include_dir
      "${GOS_EXPERIMENTAL_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND gos_experimental_gtest_include
      ${gos_experimental_unit_testing_gmock_include_dir}
      ${gos_experimental_unit_testing_gtest_include_dir})
    list(APPEND gos_experimental_gtest_libraries
      gtest
      gmock
      gmock_main)
  endif ()
  add_subdirectory(tests)
endif (BUILD_TESTS)

message(STATUS "")
message(STATUS "Geirmundur Orri Sigurdsson Experimental ${PROJECT_VERSION} BUILD SUMMARY")
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

#if (NOV_USE_BOOST_LOG)
#message(STATUS "Using Boost log")
#endif ()
#message(STATUS "  Boost version             : ${Boost_VERSION}")
#message(STATUS "  Boost include dir         : ${Boost_INCLUDE_DIRS}")
#message(STATUS "  Boost debug library dir   : ${Boost_LIBRARY_DIR_DEBUG}")
#message(STATUS "  Boost release library dir : ${Boost_LIBRARY_DIR_RELEASE}")

if (BUILD_TESTS)
message(STATUS "Building Tests")
if (MSVC_VERSION)
message(STATUS "  GTest Force Shared CRT    : ${gtest_force_shared_crt}")
endif ()
endif ()
