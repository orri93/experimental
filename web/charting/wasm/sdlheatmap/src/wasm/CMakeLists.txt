list(APPEND gos_sdl_heatmap_library_source
  chart.c
  draw.c)

list(APPEND gos_sdl_heatmap_source
  ${gos_sdl_heatmap_library_source}
  heatmap.c)

list(APPEND gos_sdl_heatmap_include
  ${gosc_include_directories}
  ${SDL_INCLUDE_DIR})

list(APPEND gos_sdl_heatmap_libraries
  ${SDL_LIBRARY}
  ${gosc_target})

#list(APPEND gos_sdl_heatmap_definitions
#  NOMINMAX)

if (GOS_SDL_MAIN)
  list(APPEND gos_sdl_heatmap_definitions
    GOS_CHART_SDL_MAIN)
  list(APPEND gos_sdl_heatmap_libraries
    ${SDLMAIN_LIBRARY})
endif (GOS_SDL_MAIN)

if (HAVE_LIB_M)
  list(APPEND gos_sdl_heatmap_libraries m)
endif (HAVE_LIB_M)

add_library(${gos_sdl_heatmap_library_target}
  ${gos_sdl_heatmap_library_source})

add_executable(${gos_sdl_heatmap_target}
  ${gos_sdl_heatmap_source})

if (gos_sdl_heatmap_definitions)
  target_compile_definitions(${gos_sdl_heatmap_library_target} PUBLIC
    ${gos_sdl_heatmap_definitions})
  target_compile_definitions(${gos_sdl_heatmap_target} PUBLIC
    ${gos_sdl_heatmap_definitions})
endif ()

target_include_directories(${gos_sdl_heatmap_library_target} PUBLIC
  ${gos_sdl_heatmap_include})
target_include_directories(${gos_sdl_heatmap_target} PUBLIC
  ${gos_sdl_heatmap_include})

target_link_libraries(${gos_sdl_heatmap_target}
  ${gos_sdl_heatmap_libraries})

if (gos_sdl_heatmap_library_public_header)
  set_target_properties(${gos_sdl_heatmap_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_sdl_heatmap_library_public_header}")
endif ()

if (BUILD_WITH_EMSCRIPTEN)
  set(EMCC_ARGS
    -I${gos_sdl_heatmap_source_include} -I${gos_c_library_include}
    -O3
    -s MODULARIZE=1
    -s EXPORT_NAME="SdlHeatmapModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  list(APPEND gos_sdl_heatmap_wasm_library_source ${gos_sdl_heatmap_source})
  list(TRANSFORM gos_sdl_heatmap_wasm_library_source
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  add_custom_command(
    TARGET ${gos_sdl_heatmap_library_target} POST_BUILD
    BYPRODUCTS gossdlheatmap.js gossdlheatmap.wasm
    COMMAND emcc ${gos_sdl_heatmap_wasm_library_source} ${gosc_wasm_library} ${EMCC_ARGS} -o gossdlheatmap.js
    DEPENDS
      ${gos_sdl_heatmap_source}
      ${gos_sdl_heatmap_library_public_header}
    COMMENT "Building GOS SDL Heatmap Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.js"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.wasm"
    DESTINATION lib)
  set(gos_assets_wasm_sdl_heatmap_target gosassetswasmsdlheatmap)
  set(gos_assets_wasm_sdl_heatmap_output
    "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.js"
    "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.wasm")
  add_custom_command(
    OUTPUT ${gos_assets_wasm_sdl_heatmap_output}
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.js" "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.js"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.wasm" "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.wasm"
    DEPENDS
      "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.js"
      "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.wasm"
    COMMENT "Coping GOS SDL Heatmap Web Assembly")
  add_custom_target(${gos_assets_wasm_sdl_heatmap_target} DEPENDS
    ${gos_assets_wasm_sdl_heatmap_output})
  add_dependencies(${gos_assets_wasm_sdl_heatmap_target}
    ${gos_sdl_heatmap_library_target})
endif ()

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_sdl_heatmap_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_sdl_heatmap_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/gos
  ARCHIVE DESTINATION lib
  RESOURCE DESTINATION lib)
install(TARGETS ${gos_sdl_heatmap_target}
  RUNTIME DESTINATION bin)
