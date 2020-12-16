set(gos_expb_modules_logistic_target logistic)

list(APPEND gos_expb_modules_logistic_source
  "logistic.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)
  find_package(SDL2_ttf COMPONENTS REQUIRED)
  find_package(PNG COMPONENTS REQUIRED)

  list(APPEND gos_expb_modules_logistic_include
    ${SDL_TTF_INCLUDE_DIRS}
    ${gos_experimental_include}
    ${gosc_include_directories}
    ${gos_expb_include})

  list(APPEND gos_expb_modules_logistic_libraries
    ${gos_expb_modules_graph_library_target}
  # ${gosc_target}
    SDL2_ttf::SDL2_ttf
    SDL2::SDL2-static
    SDL2::SDL2main
    PNG::PNG)

  add_executable(${gos_expb_modules_logistic_target}
    ${gos_expb_modules_logistic_source})

  if (gos_expb_modules_logistic_definitions)
    target_compile_definitions(${gos_expb_modules_logistic_target} PUBLIC
      ${gos_expb_modules_logistic_definitions})
  endif ()

  target_include_directories(${gos_expb_modules_logistic_target} PUBLIC
    ${gos_expb_modules_logistic_include})

  target_link_libraries(${gos_expb_modules_logistic_target}
    ${gos_expb_modules_logistic_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_logistic_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_logistic_target}
    RUNTIME DESTINATION bin)
endif()
