if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  include(GoogleTest)
endif ()

set(gos_sdl_heatmap_test_target gossdlheatmaptest)
set(gos_sdl_heatmap_test_name gossdlheatmaptest)

list(APPEND gos_sdl_heatmap_test_source
  chart.cpp)

#list(APPEND gos_sdl_heatmap_test_definitions
#  _CRT_SECURE_NO_WARNINGS)

list(APPEND gos_sdl_heatmap_test_include
  ${gos_sdl_heatmap_include}
  ${gosc_include_directories}
  ${SDL_INCLUDE_DIR})

add_executable(${gos_sdl_heatmap_test_target}
  ${gos_sdl_heatmap_test_source})

if (gos_sdl_heatmap_test_definitions)
  target_compile_definitions(${gos_sdl_heatmap_test_target} PUBLIC
    ${gos_sdl_heatmap_test_definitions})
endif ()

target_include_directories(${gos_sdl_heatmap_test_target} PUBLIC
  ${gos_sdl_heatmap_test_include})

target_link_libraries(${gos_sdl_heatmap_test_target}
  ${gos_sdl_heatmap_gtest_libraries}
  ${gos_sdl_heatmap_library_target}
  ${gosc_target})

#if(CLANG_TIDY_EXE)
#  set_target_properties(${gos_sdl_heatmap_test_target} PROPERTIES
#    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
#endif()

if (GOS_USE_GTEST_PACKAGE OR GOS_USE_GTEST_DISCOVER)
  gtest_discover_tests(${gos_sdl_heatmap_test_target}
    XML_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/xmlout)
else ()
  add_test(NAME ${gos_sdl_heatmap_test_name} COMMAND
    ${gos_sdl_heatmap_test_target})
endif ()
