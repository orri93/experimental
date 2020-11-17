set(gos_expb_modules_simple_target simple)

list(APPEND gos_expb_modules_simple_source
  "simple.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)

  list(APPEND gos_expb_modules_simple_include
    ${gosc_include_directories})

  list(APPEND gos_expb_modules_simple_libraries
    ${gosc_target}
    SDL2::SDL2-static
    SDL2::SDL2main)

  add_executable(${gos_expb_modules_simple_target}
    ${gos_expb_modules_simple_source})

  if (gos_expb_modules_simple_definitions)
    target_compile_definitions(${gos_expb_modules_simple_target} PUBLIC
      ${gos_expb_modules_simple_definitions})
  endif ()

  target_include_directories(${gos_expb_modules_simple_target} PUBLIC
    ${gos_expb_modules_simple_include})

  target_link_libraries(${gos_expb_modules_simple_target}
    ${gos_expb_modules_simple_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_simple_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_simple_target}
    RUNTIME DESTINATION bin)
endif()
