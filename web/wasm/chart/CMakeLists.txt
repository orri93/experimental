cmake_minimum_required(VERSION 3.8.2)

project(GosChartWebAssembly
  VERSION 1.0.0
  DESCRIPTION "Geirmundur Orri Sigurdsson Chart Web Assembly Angular Experiment"
  LANGUAGES C CXX)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_WITH_EMSCRIPTEN "Build a Web Assembly Library with emscripten" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

option(GOS_SDL_MAIN "Use SDL Main" OFF)

option(GOS_DEPLOY_PDB_FILES "Deploy PDB files" OFF)

option(GOS_USE_GTEST_DISCOVER "Use CMake Google Test descovery for unit tests" OFF)
option(GOS_USE_GTEST_PACKAGE "Use CMake GTest Package for unit tests" OFF)

set(GOS_BUILD_NUMBER "0" CACHE STRING "The build number")

set(gos_chart_target chart)
set(gos_chart_library_target goschart)

include(extern/gosc/cmake/gosc.cmake)
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

if (BUILD_WITH_EMSCRIPTEN)
  set(gos_wasm_chart_target goschartwasm)
  set(gos_wasm_chart_library_target goschartlibwasm)
  set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten
    CACHE STRING "Emscripten path")
  set(CMAKE_TOOLCHAIN_FILE
    ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
  set(gos_c_library_include "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/include")
endif ()

set(TIDY_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy check")
set(TIDY_FIX_ARGUMENTS "-list-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix")
set(TIDY_FIX_ERRORS_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix errors")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/version.h.in"
  "include/gos/version.h"
  @ONLY)

set(gos_chart_wasm_source_include "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(gos_chart_wasm_binary_include "${CMAKE_CURRENT_BINARY_DIR}/include")

list(APPEND gos_chart_wasm_include
  ${gos_chart_wasm_source_include}
  ${gos_chart_wasm_binary_include})

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

string(TIMESTAMP GOS_CURRENT_YEAR "%Y" UTC)
set(GOS_COMPANYNAME "Geirmundur Orri Sigurdsson")
set(GOS_LEGALCOPYRIGHT "Copyright (C) ${GOS_CURRENT_YEAR} ${GOS_COMPANYNAME}")

list(APPEND gos_wasm_chart_library_public_header
  "${CMAKE_CURRENT_BINARY_DIR}/include/gos/version.h")
list(APPEND gos_wasm_chart_library_public_header
  "${CMAKE_CURRENT_SOURCE_DIR}/include/gos/chart.h")

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

find_package(SDL REQUIRED)

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

#list(APPEND gos_chart_wasm_include
#  ${Boost_INCLUDE_DIRS})

add_subdirectory(extern/gosc)

gosc_get_properties()
 
add_subdirectory(src)

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
    if (GOS_CHART_INSTRUCT_GOOGLE_TEST_PATH)
      set(GOS_CHART_GOOGLE_TEST_PATH
        "${GOS_CHART_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(GOS_CHART_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")
#     add_subdirectory(extern/googletest)
    endif ()
    set(gos_chart_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(gos_chart_unit_testing_gmock_include_dir
      "${GOS_CHART_GOOGLE_TEST_PATH}/googlemock/include")
    set(gos_chart_unit_testing_gtest_include_dir
      "${GOS_CHART_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND gos_chart_gtest_include
      ${gos_chart_unit_testing_gmock_include_dir}
      ${gos_chart_unit_testing_gtest_include_dir})
    list(APPEND gos_chart_gtest_libraries
      gtest
      gmock
      gmock_main)
  endif ()
  add_subdirectory(tests)
endif (BUILD_TESTS)

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
