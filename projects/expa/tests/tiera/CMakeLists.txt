if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  include(GoogleTest)
endif ()

set(gos_tier_a_test_target gostieratest)
set(gos_tier_a_test_name gostieratest)

list(APPEND gos_tier_a_test_source
  tiera.cpp)

#list(APPEND gos_tier_a_test_definitions
#  _CRT_SECURE_NO_WARNINGS)

list(APPEND gos_tier_a_test_include
  ${gosc_include_directories}
  ${gos_expa_gtest_include}
  ${gos_expa_include})

add_executable(${gos_tier_a_test_target}
  ${gos_tier_a_test_source})

if (gos_tier_a_test_definitions)
  target_compile_definitions(${gos_tier_a_test_target} PUBLIC
    ${gos_tier_a_test_definitions})
endif ()

target_include_directories(${gos_tier_a_test_target} PUBLIC
  ${gos_tier_a_test_include})

target_link_libraries(${gos_tier_a_test_target}
  ${gos_expa_gtest_libraries}
  ${gos_tier_a_library_target}
  ${gosc_target})

#if(CLANG_TIDY_EXE)
#  set_target_properties(${gos_tier_a_test_target} PROPERTIES
#    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
#endif()

if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  gtest_discover_tests(${gos_tier_a_test_target}
    XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/xmlout)
else ()
  add_test(NAME ${gos_tier_a_test_name} COMMAND
    ${gos_tier_a_test_target})
endif ()
