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
option(GOS_QT_EXPERIMENTAL "GOS Experimental for Qt" OFF)

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

if (GOS_NN_EXPERIMENTAL)
  set(gos_nn_experimental_opennn_dir "${CMAKE_SOURCE_DIR}/extern/opennn")
  set(gos_nn_experimental_opennn_library opennn)
  list(APPEND gos_nn_experimental_opennn_include
    "${gos_nn_experimental_opennn_dir}/opennn"
    "${gos_nn_experimental_opennn_dir}")
  add_subdirectory(extern/opennn)
endif ()

if (GOS_SPB_EXPERIMENTAL)
  set(gos_spb_experimental_nanopb_dir "${CMAKE_SOURCE_DIR}/extern/nanopb")
  set(gos_spb_experimental_nanopb_library protobuf-nanopb-static)
  set(gos_spb_experimental_nanopb_generator_py "${gos_spb_experimental_nanopb_dir}/generator/nanopb_generator.py")
  add_subdirectory(extern/nanopb)
  
  set(gos_spb_experimental_tahu_dir "${CMAKE_SOURCE_DIR}/extern/tahu")
  set(gos_spb_experimental_tahu_library tahuspb)
  list(APPEND gos_spb_experimental_include
    ${gos_spb_experimental_nanopb_dir})
endif ()

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
