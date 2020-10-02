list(APPEND gos_wasm_chart_library_source
  chart.c)

list(APPEND gos_wasm_chart_library_definitions
# _CRT_SECURE_NO_WARNINGS
  _USE_MATH_DEFINES)

list(APPEND gos_wasm_chart_library_include
  ${gosc_include_directories}
  ${gos_chart_wasm_include})

add_library(${gos_chart_library_target}
  ${gos_wasm_chart_library_source})

add_dependencies(${gos_chart_library_target} ${gosc_target})

if (gos_wasm_chart_library_definitions)
  target_compile_definitions(${gos_chart_library_target} PUBLIC
    ${gos_wasm_chart_library_definitions})
endif ()

target_include_directories(${gos_chart_library_target} PUBLIC
  ${gos_wasm_chart_library_include})

if (gos_wasm_chart_library_public_header)
  set_target_properties(${gos_chart_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_wasm_chart_library_public_header}")
endif ()

if (BUILD_WITH_EMSCRIPTEN)
  set(EMCC_ARGS
    -I${gos_chart_wasm_source_include} -I${gos_c_library_include}
    -O3)
  list(APPEND gos_chart_wasm_library_source ${gos_wasm_chart_library_source})
  list(TRANSFORM gos_chart_wasm_library_source
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  add_custom_command(
    #OUTPUT libgoschart.bc
    TARGET ${gos_chart_library_target} POST_BUILD
    COMMAND emcc ${gos_chart_wasm_library_source} ${EMCC_ARGS} -o libgoschart.bc
    DEPENDS
      ${gos_wasm_chart_library_source}
      ${gos_wasm_chart_library_public_header}
    COMMENT "Building GOS Chart Web Assembly Library"
    VERBATIM)
  #add_custom_target(${gos_wasm_chart_library_target} ALL DEPENDS libgoschart.bc)
  set_target_properties(${gos_chart_library_target} PROPERTIES
    RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/libgoschart.bc")
  #install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libgoschart.bc" DESTINATION lib)
endif ()

install(TARGETS ${gos_chart_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/gos
  ARCHIVE DESTINATION lib
  RESOURCE DESTINATION lib)
