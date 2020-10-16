list(APPEND gos_expa_heatmap_library_source
  chart.c
  draw.c)

list(APPEND gos_expa_heatmap_source
  ${gos_expa_heatmap_library_source}
  heatmap.c)

list(APPEND gos_expa_heatmap_include
  ${gosc_include_directories}
  ${gos_expa_include})

#list(APPEND gos_expa_heatmap_definitions
#  NOMINMAX)

list(APPEND gos_expa_heatmap_libraries
  ${gosc_target})

if (GOS_SDL_TARGET)
  list(APPEND gos_expa_heatmap_include
    ${SDL_INCLUDE_DIR})
  list(APPEND gos_expa_heatmap_libraries
    ${SDL_LIBRARY})
  if (GOS_SDL_MAIN)
    list(APPEND gos_expa_heatmap_definitions
      GOS_CHART_SDL_MAIN)
    list(APPEND gos_expa_heatmap_libraries
      ${SDLMAIN_LIBRARY})
  endif (GOS_SDL_MAIN)
endif ()

if (HAVE_LIB_M)
  list(APPEND gos_expa_heatmap_libraries m)
endif (HAVE_LIB_M)

if (GOS_DEVELOPE_EMSCRIPTEN)
  list(APPEND gos_expa_heatmap_definitions
    __EMSCRIPTEN__)
  list(APPEND gos_expa_heatmap_include
    ${gos_emscripten_cjson_lib_dir}
    ${gos_emscripten_include})
endif ()

if (GOS_SDL_TARGET)
  add_library(${gos_expa_heatmap_library_target}
    ${gos_expa_heatmap_library_source})

  add_executable(${gos_expa_heatmap_target}
    ${gos_expa_heatmap_source})

  if (gos_expa_heatmap_definitions)
    target_compile_definitions(${gos_expa_heatmap_library_target} PUBLIC
      ${gos_expa_heatmap_definitions})
    target_compile_definitions(${gos_expa_heatmap_target} PUBLIC
      ${gos_expa_heatmap_definitions})
  endif ()

  target_include_directories(${gos_expa_heatmap_library_target} PUBLIC
    ${gos_expa_heatmap_include})
  target_include_directories(${gos_expa_heatmap_target} PUBLIC
    ${gos_expa_heatmap_include})

  target_link_libraries(${gos_expa_heatmap_target}
    ${gos_expa_heatmap_libraries})

  if (gos_expa_heatmap_library_public_header)
    set_target_properties(${gos_expa_heatmap_library_target}
      PROPERTIES PUBLIC_HEADER
      "${gos_expa_heatmap_library_public_header}")
  endif ()

else ()

  add_library(${gos_expa_heatmap_library_target}
    dummy.c)

endif ()

if (BUILD_WITH_EMSCRIPTEN)
  if (GOS_EM_ASSERTIONS)
    set(em_assertions 1)
  else ()
    set(em_assertions 0)
  endif ()
  if (GOS_EMSCRIPTEN_CJSON_LIB)
    set(em_include -I${gos_expa_source_include} -I${gos_c_library_include} -I${gos_emscripten_cjson_lib_dir})
  else ()
    set(em_include -I${gos_expa_source_include} -I${gos_c_library_include})
  endif ()
  set(EMCC_ARGS
    ${em_include}
    -O3
    -s MODULARIZE=1
    -s ASSERTIONS=${em_assertions}
    -s EXPORT_NAME="SdlHeatmapModule"
    -s EXTRA_EXPORTED_RUNTIME_METHODS="['ccall']")
  list(APPEND gos_expa_heatmap_wasm_library_source ${gos_expa_heatmap_source})
  list(TRANSFORM gos_expa_heatmap_wasm_library_source
    PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
  add_custom_command(
    TARGET ${gos_expa_heatmap_library_target} POST_BUILD
    BYPRODUCTS gossdlheatmap.js gossdlheatmap.wasm
    COMMAND emcc ${gos_expa_heatmap_wasm_library_source} ${gosc_wasm_library} ${EMCC_ARGS} -o gossdlheatmap.js
    DEPENDS
      ${gos_expa_heatmap_source}
      ${gos_expa_heatmap_library_public_header}
    COMMENT "Building GOS SDL Heatmap Web Assembly")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.js"
    DESTINATION lib)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.wasm"
    DESTINATION lib)
  #add_custom_command(
  #  TARGET ${gos_expa_heatmap_library_target} POST_BUILD
  #  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.js" "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.js"
  #  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/gossdlheatmap.wasm" "${gos_assets_wasm_sdl_heatmap_dir}/gossdlheatmap.wasm"
  #  DEPENDS gossdlheatmap.js gossdlheatmap.wasm
  #  COMMENT "Coping GOS SDL Heatmap Web Assembly")
endif ()

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_expa_heatmap_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_expa_heatmap_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/gos
  ARCHIVE DESTINATION lib
  RESOURCE DESTINATION lib)

if (GOS_SDL_TARGET)
  install(TARGETS ${gos_expa_heatmap_target}
    RUNTIME DESTINATION bin)
endif ()
