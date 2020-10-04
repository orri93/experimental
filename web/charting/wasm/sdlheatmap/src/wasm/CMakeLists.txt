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
#   CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
