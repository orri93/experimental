if (USE_GTEST_PACKAGE OR USE_GTEST_DISCOVER)
  include(GoogleTest)
endif ()

set(wd3_hm_test_target wd3hmtest)
set(wd3_hm_test_name wd3hmtest)

find_package(SDL REQUIRED)

list(APPEND wd3_hm_test_source
  data.cpp
  sdlmock.c)

list(APPEND wd3_hm_test_include
  ${wd3_experiment_c_gtest_include}
  ${gosc_include_directories}
  ${wd3_experiment_c_include})

if (SDL_INCLUDE_DIR)
  message(STATUS "  SDL Include detected      : ${SDL_INCLUDE_DIR}")
  list(APPEND wd3_hm_test_include
    "${SDL_INCLUDE_DIR}")
elseif (EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR)
  message(STATUS "  EMSDK Include detected    : ${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}")
  list(APPEND wd3_hm_test_include
    "${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}/SDL")
  #list(APPEND wd3_hm_test_definitions
  #  HAVE_STDINT_H)
endif ()

list(APPEND wd3_hm_test_libraries
  ${wd3_experiment_c_modules_hm_testable_target}
  ${wd3_experiment_c_gtest_libraries}
  ${gosc_target})

# if (HAVE_LIB_M)
#   list(APPEND wd3_hm_test_libraries m)
# endif (HAVE_LIB_M)

add_executable(${wd3_hm_test_target}
  ${wd3_hm_test_source})

if (wd3_hm_test_definitions)
  target_compile_definitions(${wd3_hm_test_target} PUBLIC
    ${wd3_hm_test_definitions})
endif ()

target_include_directories(${wd3_hm_test_target} PUBLIC
  ${wd3_hm_test_include})

target_link_libraries(${wd3_hm_test_target}
  ${wd3_hm_test_libraries})

#if(CLANG_TIDY_EXE)
#  set_target_properties(${wd3_hm_test_target} PROPERTIES
#    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
#endif()

if (USE_GTEST_PACKAGE OR USE_GTEST_DISCOVER)
  gtest_discover_tests(${wd3_hm_test_target}
    XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/xmlout)
else ()
  add_test(NAME ${wd3_hm_test_name} COMMAND
    ${wd3_hm_test_target})
endif ()
