if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  include(GoogleTest)
endif ()

set(gos_expa_wasm_test_target gosexpawasmtest)
set(gos_expa_wasm_test_name gosexpawasmtest)

list(APPEND gos_expa_wasm_test_source
  ${gos_emscripten_cjson_src_dir}/cJSON.c
  performance.cpp
  json.cpp)

#list(APPEND gos_tier_a_test_definitions
#  _CRT_SECURE_NO_WARNINGS)

list(APPEND gos_expa_wasm_test_include
  ${gos_emscripten_cjson_src_dir}
  ${gosc_include_directories}
  ${gos_expa_gtest_include}
  ${gos_expa_include})

add_executable(${gos_expa_wasm_test_target}
  ${gos_expa_wasm_test_source})

if (gos_expa_wasm_test_definitions)
  target_compile_definitions(${gos_expa_wasm_test_target} PUBLIC
    ${gos_expa_wasm_test_definitions})
endif ()

target_include_directories(${gos_expa_wasm_test_target} PUBLIC
  ${gos_expa_wasm_test_include})

target_link_libraries(${gos_expa_wasm_test_target}
  ${gos_expa_gtest_libraries}
  ${gos_expa_testable_library_target}
  ${gosc_target})

#if(CLANG_TIDY_EXE)
#  set_target_properties(${gos_expa_wasm_test_target} PROPERTIES
#    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
#endif()

if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  gtest_discover_tests(${gos_expa_wasm_test_target}
    XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/xmlout)
else ()
  add_test(NAME ${gos_expa_wasm_test_name} COMMAND
    ${gos_expa_wasm_test_target})
endif ()
