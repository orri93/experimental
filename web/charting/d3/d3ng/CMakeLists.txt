cmake_minimum_required(VERSION 3.8.2)

project(GosExperimentD3
  VERSION 1.0.0
  DESCRIPTION "Geirmundur Orri Sigurdsson Experiment D3"
  LANGUAGES C CXX)

# Build Options
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Tidy Options
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

# GOS Options
option(GOS_SDL_DESKTOP_TARGET "Generate SDL Desktop targets" OFF)
option(GOS_DEPLOY_PDB_FILES "Deploy PDB files" OFF)
option(GOS_EM_TARGET "Emscripten Target" OFF)
option(GOS_EM_ASSERTIONS "EMCC ASSERTIONS" OFF)

# GOS Test Options
option(GOS_USE_GTEST_DISCOVER
  "Use CMake Google Test descovery for unit tests" OFF)
option(GOS_USE_GTEST_PACKAGE
  "Use CMake GTest Package for unit tests" OFF)
option(GOS_DEVELOPE_EMSCRIPTEN
  "Develope with emscripten" OFF)

set(GOS_BUILD_NUMBER "0" CACHE STRING "The build number")

set(gos_d3ng_source_include "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(gos_d3ng_binary_include "${CMAKE_CURRENT_BINARY_DIR}/include")

list(APPEND gos_d3ng_include
  ${gos_d3ng_source_include}
  ${gos_d3ng_binary_include})

include(extern/gosc/cmake/gosc.cmake)

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

set(gos_d3ng_em_modules_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

if (GOS_EM_TARGET)
  message(STATUS "Building with Emscripten")
  if (GOS_EM_ASSERTIONS)
    set(gos_d3ng_em_assertions 1)
  else ()
    set(gos_d3ng_em_assertions 0)
  endif ()
  message(STATUS "  Assertions              : ${gos_d3ng_em_assertions}")

  # The Heat-map WebAssembly Module
  set(gos_d3ng_em_hm_target hmwasm)
  list(APPEND gos_d3ng_em_hm_source
    "${gos_d3ng_em_modules_dir}/hm/hmm.c"
    "${gos_d3ng_em_modules_dir}/hm/hm.c"
    ${gosc_source})
  list(APPEND gos_d3ng_em_hm_include
    ${gosc_include_directories}
    ${gos_d3ng_source_include})
  list(TRANSFORM gos_d3ng_em_hm_include PREPEND -I)
  set(gos_d3ng_em_hm_args
    ${gos_d3ng_em_hm_include}
    -O3
    -s MODULARIZE=1
    -s USE_SDL=2
    -s TOTAL_MEMORY=67108864
    -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0
    -s ASSERTIONS=${gos_d3ng_em_assertions}
    -s EXPORT_NAME="HmModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  set(gos_d3ng_em_hm_js hm.js)
  set(gos_d3ng_em_hm_wasm hm.wasm)
  add_custom_target(${gos_d3ng_em_hm_target} ALL
    COMMAND emcc ${gos_d3ng_em_hm_args} -o ${gos_d3ng_em_hm_js} ${gos_d3ng_em_hm_source}
    BYPRODUCTS ${gos_d3ng_em_hm_js} ${gos_d3ng_em_hm_wasm}
    DEPENDS ${gos_d3ng_em_hm_source}
    COMMENT "Building GOS SDL M-Set Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${gos_d3ng_em_hm_js}"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${gos_d3ng_em_hm_wasm}"
    DESTINATION lib)
endif ()

if (BUILD_TESTS)
  message(STATUS "Building Tests")
  if (MSVC_VERSION)
    message(STATUS "  GTest Force Shared CRT    : ${gtest_force_shared_crt}")
  endif ()

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
    if (GOS_EXPB_INSTRUCT_GOOGLE_TEST_PATH)
      set(GOS_EXPB_GOOGLE_TEST_PATH
        "${GOS_EXPB_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(GOS_EXPB_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")
#     add_subdirectory(extern/googletest)
    endif ()
    set(gos_expb_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(gos_expb_unit_testing_gmock_include_dir
      "${GOS_EXPB_GOOGLE_TEST_PATH}/googlemock/include")
    set(gos_expb_unit_testing_gtest_include_dir
      "${GOS_EXPB_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND gos_expb_gtest_include
      ${gos_expb_unit_testing_gmock_include_dir}
      ${gos_expb_unit_testing_gtest_include_dir})
    list(APPEND gos_expb_gtest_libraries
      gtest
      gmock
      gmock_main)
  endif ()
  #add_subdirectory(tests)
endif (BUILD_TESTS)
