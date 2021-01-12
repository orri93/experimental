set(gos_d3ng_modules_hm_target hm)

list(APPEND gos_d3ng_modules_hm_source
  hm.c)

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)

  message(STATUS "SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")

  list(APPEND gos_d3ng_modules_hm_include
    ${SDL_TTF_INCLUDE_DIRS}
    ${gosc_include_directories}
    ${gos_d3ng_include})

  list(APPEND gos_d3ng_modules_hm_libraries
    ${gosc_target}
    SDL2::SDL2-static
    SDL2::SDL2main)

  if (MSVC)
    list(APPEND gos_d3ng_modules_hm_definitions
      HAVE_M_PI)
  endif (MSVC)

  add_executable(${gos_d3ng_modules_hm_target}
    ${gos_d3ng_modules_hm_source})

  if (gos_d3ng_modules_hm_definitions)
    target_compile_definitions(${gos_d3ng_modules_hm_target} PUBLIC
      ${gos_d3ng_modules_hm_definitions})
  endif ()

  target_include_directories(${gos_d3ng_modules_hm_target} PUBLIC
    ${gos_d3ng_modules_hm_include})

  target_link_libraries(${gos_d3ng_modules_hm_target}
    ${gos_d3ng_modules_hm_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_d3ng_modules_hm_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_d3ng_modules_hm_target}
    RUNTIME DESTINATION bin)
endif()
