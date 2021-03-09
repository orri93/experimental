cmake_minimum_required(VERSION 3.8.2)

project(GosWd3Experiment
  VERSION 1.0.0
  DESCRIPTION "GOS WebAssembly D3 Heatmap Charting Experiment"
  LANGUAGES C CXX)

# Build Options
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Tidy Options
option(CLANG_TIDY_FIX_ERRORS
  "Perform fixes with Clang-Tidy even if compilation errors were found" OFF)
option(CLANG_TIDY_FIX "Perform fixes with Clang-Tidy" OFF)
option(CLANG_TIDY "Perform Clang-Tidy check" OFF)

#  Options
option(SDL_DESKTOP_TARGET "Generate SDL Desktop targets" OFF)
option(DEPLOY_PDB_FILES "Deploy PDB files" OFF)
option(EM_TARGET "Emscripten Target" OFF)
option(EM_ASSERTIONS "EMCC ASSERTIONS" OFF)

# Test Options
option(USE_GTEST_DISCOVER
  "Use CMake Google Test discovery for unit tests" OFF)
option(USE_GTEST_PACKAGE
  "Use CMake GTest Package for unit tests" OFF)
option(DEVELOPE_EMSCRIPTEN
  "Develop with emscripten" OFF)

# SDL Options
if (SDL_DESKTOP_TARGET)
  option(USE_SDL_MAIN "Use SDL Main for the Desktop Demo" ON)
endif (SDL_DESKTOP_TARGET)

set(BUILD_NUMBER "0" CACHE STRING "The build number")

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

set(GOS_CPP_INSTRUCT_GOOGLE_TEST_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")

add_subdirectory(extern/gosc)
add_subdirectory(extern/goscpp)
add_subdirectory(extern/date)

include(extern/gosc/cmake/gosc.cmake)
include(extern/goscpp/cmake/goscpp.cmake)

gosc_get_properties()
goscpp_get_properties()

set(wd3_experiment_extern_date_dir
  "${CMAKE_CURRENT_SOURCE_DIR}/extern/date")
set(wd3_experiment_extern_date_include
  "${wd3_experiment_extern_date_dir}/include")

set(wd3_experiment_c_source_include "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(wd3_experiment_c_binary_include "${CMAKE_CURRENT_BINARY_DIR}/include")

list(APPEND wd3_experiment_c_include
  ${wd3_experiment_c_source_include}
  ${wd3_experiment_c_binary_include})

if(BUILD_TESTS)
  set(wd3_experiment_c_modules_hm_testable_target testablehm)
  set(wd3_experiment_cpp_modules_hm_testable_target testablehmpp)
endif()

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

set(wd3_experiment_c_em_modules_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

if (EM_TARGET)
  message(STATUS "Building with Emscripten")
  if (EM_ASSERTIONS)
    set(wd3_experiment_c_em_assertions 1)
  else ()
    set(wd3_experiment_c_em_assertions 0)
  endif ()
  message(STATUS "  Assertions                : ${wd3_experiment_c_em_assertions}")

  # The Heat-map C WebAssembly Module
  set(wd3_experiment_c_em_hm_target hmwasm)
  list(APPEND wd3_experiment_c_em_hm_source
    "${wd3_experiment_c_em_modules_dir}/hm/hmm.c"
    "${wd3_experiment_c_em_modules_dir}/hm/hm.c"
    ${gosc_source})
  list(APPEND wd3_experiment_c_em_hm_include
    ${gosc_include_directories}
    ${wd3_experiment_c_source_include})
  list(TRANSFORM wd3_experiment_c_em_hm_include PREPEND -I)
  set(wd3_experiment_c_em_hm_args
    ${wd3_experiment_c_em_hm_include}
    -O3
    -s MODULARIZE=1
#   -s USE_SDL=2
    -s TOTAL_MEMORY=67108864
#   -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0
    -s ASSERTIONS=${wd3_experiment_c_em_assertions}
    -s EXPORT_NAME="HmModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  set(wd3_experiment_c_em_hm_js hm.js)
  set(wd3_experiment_c_em_hm_wasm hm.wasm)
  add_custom_target(${wd3_experiment_c_em_hm_target} ALL
    COMMAND emcc ${wd3_experiment_c_em_hm_args} -o ${wd3_experiment_c_em_hm_js} ${wd3_experiment_c_em_hm_source}
    BYPRODUCTS ${wd3_experiment_c_em_hm_js} ${wd3_experiment_c_em_hm_wasm}
    DEPENDS ${wd3_experiment_c_em_hm_source}
    COMMENT "Building the WD3 C SDL Heatmap Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_c_em_hm_js}"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_c_em_hm_wasm}"
    DESTINATION lib)

  # The Heat-map C++ WebAssembly Module
  set(wd3_experiment_cpp_em_hmpp_target hmppwasm)
  list(APPEND wd3_experiment_cpp_em_hmpp_source
    "${wd3_experiment_c_em_modules_dir}/hmpp/emscripten.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/exception.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/gradient.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/context.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/column.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/global.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/point.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/data.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/time.cpp"
    "${wd3_experiment_c_em_modules_dir}/hmpp/hm.cpp"
    ${goscpp_source})
  list(APPEND wd3_experiment_cpp_em_hmpp_include
    ${goscpp_include_directories}
    ${wd3_experiment_c_source_include}
    ${wd3_experiment_extern_date_include})
  list(TRANSFORM wd3_experiment_cpp_em_hmpp_include PREPEND -I)
  set(wd3_experiment_cpp_em_hmpp_args
    ${wd3_experiment_cpp_em_hmpp_include}
    -std=c++17
    -O3
    -s MODULARIZE=1
#   -s USE_SDL=2
    -s TOTAL_MEMORY=67108864
#   -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0
    -s ASSERTIONS=${wd3_experiment_c_em_assertions}
    -s EXPORT_NAME="HmppModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  set(wd3_experiment_cpp_em_hmpp_js hmpp.js)
  set(wd3_experiment_cpp_em_hmpp_wasm hmpp.wasm)
  add_custom_target(${wd3_experiment_cpp_em_hmpp_target} ALL
    COMMAND em++ ${wd3_experiment_cpp_em_hmpp_args} -o ${wd3_experiment_cpp_em_hmpp_js} ${wd3_experiment_cpp_em_hmpp_source}
    BYPRODUCTS ${wd3_experiment_cpp_em_hmpp_js} ${wd3_experiment_cpp_em_hmpp_wasm}
    DEPENDS ${wd3_experiment_cpp_em_hmpp_source}
    COMMENT "Building the WD3 C++ SDL Heatmap Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_cpp_em_hmpp_js}"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_cpp_em_hmpp_wasm}"
    DESTINATION lib)

  # The Demo WebAssembly Module
  set(wd3_experiment_c_em_demo_target demowasm)
  list(APPEND wd3_experiment_c_em_demo_source
    "${wd3_experiment_c_em_modules_dir}/demo/demo.c"
    ${gosc_source})
  list(APPEND wd3_experiment_c_em_demo_include
    ${gosc_include_directories}
    ${wd3_experiment_c_source_include})
  list(TRANSFORM wd3_experiment_c_em_demo_include PREPEND -I)
  set(wd3_experiment_c_em_demo_args
    ${wd3_experiment_c_em_demo_include}
    -O3
    -s MODULARIZE=1
    -s TOTAL_MEMORY=67108864
    -s ASSERTIONS=${wd3_experiment_c_em_assertions}
    -s EXPORT_NAME="DemoModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  set(wd3_experiment_c_em_demo_js demo.js)
  set(wd3_experiment_c_em_demo_wasm demo.wasm)
  add_custom_target(${wd3_experiment_c_em_demo_target} ALL
    COMMAND emcc ${wd3_experiment_c_em_demo_args} -o ${wd3_experiment_c_em_demo_js} ${wd3_experiment_c_em_demo_source}
    BYPRODUCTS ${wd3_experiment_c_em_demo_js} ${wd3_experiment_c_em_demo_wasm}
    DEPENDS ${wd3_experiment_c_em_demo_source}
    COMMENT "Building WD3 SDL Demo Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_c_em_demo_js}"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${wd3_experiment_c_em_demo_wasm}"
    DESTINATION lib)
endif ()

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
    if (WD3_INSTRUCT_GOOGLE_TEST_PATH)
      set(WD3_GOOGLE_TEST_PATH
        "${WD3_INSTRUCT_GOOGLE_TEST_PATH}")
    else ()
      set(WD3_GOOGLE_TEST_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}/extern/gosc/extern/googletest")
#     add_subdirectory(extern/googletest)
    endif ()
    set(wd3_experiment_c_unit_testing_dir
      "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(wd3_experiment_c_unit_testing_gmock_include_dir
      "${WD3_GOOGLE_TEST_PATH}/googlemock/include")
    set(wd3_experiment_c_unit_testing_gtest_include_dir
      "${WD3_GOOGLE_TEST_PATH}/googletest/include")
    list(APPEND wd3_experiment_c_gtest_include
      ${wd3_experiment_c_unit_testing_gmock_include_dir}
      ${wd3_experiment_c_unit_testing_gtest_include_dir})
    list(APPEND wd3_experiment_c_gtest_libraries
      gtest
      gmock
      gmock_main)
  endif ()
  add_subdirectory(tests)
endif (BUILD_TESTS)
