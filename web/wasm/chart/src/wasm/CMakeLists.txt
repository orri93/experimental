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

if (BUILD_WITH_EMSCRIPTEN)
  get_target_property(gos_chart_library_wasm ${gos_chart_library_target}
    RESOURCE)
  set(EMCC_ARGS
    -I${gos_chart_wasm_source_include} -I${gos_c_library_include}
    -O3
    -s MODULARIZE=1
    -s EXPORT_NAME="ChartModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  list(APPEND gos_chart_wasm_source heatmap.c draw.c)
  list(TRANSFORM gos_chart_wasm_source
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  add_custom_command(
    #OUTPUT goschart.js
    #BYPRODUCTS goschart.wasm
    TARGET ${gos_chart_target} POST_BUILD
    COMMAND emcc ${gos_chart_wasm_source} ${gosc_wasm_library} ${gos_chart_library_wasm} ${EMCC_ARGS} -o goschart.js
    DEPENDS
      #${gosc_wasm_library}
      #${gos_chart_library_wasm}
      ${gos_chart_wasm_source}
    COMMENT "Building GOS Chart Web Assembly"
    #VERBATIM
  )
  #add_custom_target(${gos_wasm_chart_target} ALL DEPENDS goschart.js)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/goschart.js" DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/goschart.wasm" DESTINATION lib)
endif ()

if (gos_chart_definitions)
  target_compile_definitions(${gos_chart_target} PUBLIC
    ${gos_chart_definitions})
endif ()

target_include_directories(${gos_chart_target} PUBLIC
  ${gos_chart_include})

target_link_libraries(${gos_chart_target}
  ${gos_chart_libraries}
  ${gosc_target})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_chart_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_chart_target}
#   CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
