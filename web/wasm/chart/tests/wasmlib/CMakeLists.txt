if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  include(GoogleTest)
endif ()

set(gos_chart_test_target goscharttest)
set(gos_chart_test_name goscharttest)

list(APPEND gos_chart_test_source
  chart.cpp)

#list(APPEND gos_chart_test_definitions
#  _CRT_SECURE_NO_WARNINGS)

list(APPEND gos_chart_test_include
  ${gos_chart_gtest_include}
  ${gos_chart_wasm_include})

add_executable(${gos_chart_test_target}
  ${gos_chart_test_source})

add_dependencies(${gos_chart_test_target} gosc)

if (gos_chart_test_definitions)
  target_compile_definitions(${gos_chart_test_target} PUBLIC
    ${gos_chart_test_definitions})
endif ()

target_include_directories(${gos_chart_test_target} PUBLIC
  ${gos_chart_test_include})

target_link_libraries(${gos_chart_test_target}
  ${gos_chart_gtest_libraries}
  ${gos_chart_library_target}
  ${gosc_target})

#if(CLANG_TIDY_EXE)
#  set_target_properties(${gos_chart_test_target} PROPERTIES
#    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
#endif()

if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  gtest_discover_tests(${gos_chart_test_target}
    XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/xmlout)
else ()
  add_test(NAME ${gos_chart_test_name} COMMAND
    ${gos_chart_test_target})
endif ()
