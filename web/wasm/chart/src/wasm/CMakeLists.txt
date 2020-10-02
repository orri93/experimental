list(APPEND gos_chart_source
  experimental.c
  development.c
  heatmap.c
  draw.c)

list(APPEND gos_chart_include
  ${gos_chart_wasm_include})

list(APPEND gos_chart_libraries
  ${gos_chart_library_target}
  ${gosc_target})

#list(APPEND gos_chart_definitions
#  NOMINMAX)

if (SDL_FOUND)
  list(APPEND gos_chart_include
    ${SDL_INCLUDE_DIR})
  list(APPEND gos_chart_libraries
    ${SDL_LIBRARY})

  if (GOS_SDL_MAIN)
    list(APPEND gos_chart_definitions
      GOS_CHART_SDL_MAIN)
    list(APPEND gos_chart_libraries
      ${SDLMAIN_LIBRARY})
  endif (GOS_SDL_MAIN)

  add_executable(${gos_chart_target}
    ${gos_chart_source})

  target_compile_definitions(${gos_chart_target} PUBLIC
    ${gos_chart_definitions})

  target_include_directories(${gos_chart_target} PUBLIC
    ${gos_chart_include})

  target_link_libraries(${gos_chart_target}
    ${gos_chart_libraries}
    gosc)

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_chart_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_chart_target}
#   CONFIGURATIONS etc
    RUNTIME DESTINATION bin)
endif (SDL_FOUND)

if (BUILD_WITH_EMSCRIPTEN)
  set(EMCC_ARGS
    -I${gosc_include_directories} -I${gos_chart_wasm_source_include}
    -O3)
  list(APPEND gos_chart_wasm_source heatmap.c draw.c)
  list(TRANSFORM gos_chart_wasm_source
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  add_custom_command(
    OUTPUT goschart.wasm
    COMMAND emcc ${gos_chart_wasm_source} ${EMCC_ARGS} -o goschart.wasm
    DEPENDS
      ${gos_chart_wasm_source}
    COMMENT "Building GOS Chart Web Assembly"
    VERBATIM)
  add_custom_target(${gos_wasm_chart_library_library_target}
    ALL DEPENDS libgoschart.bc)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/goschart.wasm" DESTINATION lib)
endif ()
