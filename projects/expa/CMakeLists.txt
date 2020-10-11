cmake_minimum_required(VERSION 3.8.2)

project(GosExperimentA
  VERSION 1.0.0
  DESCRIPTION "Geirmundur Orri Sigurdsson Experiment Project A"
  LANGUAGES C CXX)

# Build Options
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_WITH_EMSCRIPTEN "Build a Web Assembly Library with emscripten" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Tidy Options
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

# GOS Options
option(GOS_SDL_MAIN "Use SDL Main" OFF)
option(GOS_DEPLOY_PDB_FILES "Deploy PDB files" OFF)
option(GOS_EM_ASSERTIONS "EMCC ASSERTIONS" OFF)

# GOS Test Options
option(GOS_USE_GTEST_DISCOVER
  "Use CMake Google Test descovery for unit tests" OFF)
option(GOS_USE_GTEST_PACKAGE
  "Use CMake GTest Package for unit tests" OFF)

set(GOS_BUILD_NUMBER "0" CACHE STRING "The build number")

set(gos_tier_a_library_target tiera)
list(APPEND gos_tier_a_library_public_header
  "${CMAKE_CURRENT_BINARY_DIR}/include/tiera/versioni.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tiera/tiera.h")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/tiera/version.h.in"
  "include/tiera/version.h"
  @ONLY)

set(gos_expa_source_include "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(gos_expa_binary_include "${CMAKE_CURRENT_BINARY_DIR}/include")

list(APPEND gos_expa_include
  ${gos_expa_source_include}
  ${gos_expa_binary_include})

include(extern/gosc/cmake/gosc.cmake)
include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(m sin "" HAVE_LIB_M)

if (BUILD_WITH_EMSCRIPTEN)
  set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten
    CACHE STRING "Emscripten path")
  set(CMAKE_TOOLCHAIN_FILE
    ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
  set(gos_c_library_include "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/include")
  set(gos_assets_wasm_sdl_heatmap_dir
    "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/wasm")
endif ()

set(TIDY_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy check")
set(TIDY_FIX_ARGUMENTS "-list-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix")
set(TIDY_FIX_ERRORS_ARGUMENTS "-checks=*,-clang-analyzer-alpha.* "
  CACHE STRING "Arguments for Clang-Tidy fix errors")

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
    if (GOS_EXPA_INSTRUCT_GOOGLE_TEST_PATH)
      set(GOS_EXPA_GOOGLE_TEST_PATH
        "${GOS_EXPA_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(GOS_EXPA_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")
#     add_subdirectory(extern/googletest)
    endif ()
    set(gos_expa_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(gos_expa_unit_testing_gmock_include_dir
      "${GOS_EXPA_GOOGLE_TEST_PATH}/googlemock/include")
    set(gos_expa_unit_testing_gtest_include_dir
      "${GOS_EXPA_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND gos_expa_gtest_include
      ${gos_expa_unit_testing_gmock_include_dir}
      ${gos_expa_unit_testing_gtest_include_dir})
    list(APPEND gos_expa_gtest_libraries
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

if (BUILD_TESTS)
message(STATUS "Building Tests")
if (MSVC_VERSION)
message(STATUS "  GTest Force Shared CRT    : ${gtest_force_shared_crt}")
endif ()
endif ()
