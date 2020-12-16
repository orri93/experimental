set(gos_expb_modules_owl_target owl)

list(APPEND gos_expb_modules_owl_source
  "owl.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)
  find_package(SDL2_ttf COMPONENTS REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(PNG COMPONENTS REQUIRED)

  list(APPEND gos_expb_modules_owl_include
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL_TTF_INCLUDE_DIRS})

  list(APPEND gos_expb_modules_owl_libraries
    ${SDL2_IMAGE_LIBRARIES}
    SDL2_ttf::SDL2_ttf
    SDL2::SDL2main
    SDL2::SDL2
    PNG::PNG)

  if (MSVC)
    list(APPEND gos_expb_modules_owl_options
      /wd4244)
  endif ()

  add_executable(${gos_expb_modules_owl_target}
    ${gos_expb_modules_owl_source})

  if (gos_expb_modules_owl_definitions)
    target_compile_definitions(${gos_expb_modules_owl_target} PUBLIC
      ${gos_expb_modules_owl_definitions})
  endif ()

  if (gos_expb_modules_owl_options)
    target_compile_options(${gos_expb_modules_owl_target} PUBLIC
      ${gos_expb_modules_owl_options})
  endif ()

  target_include_directories(${gos_expb_modules_owl_target} PUBLIC
    ${gos_expb_modules_owl_include})

  target_link_libraries(${gos_expb_modules_owl_target}
    ${gos_expb_modules_owl_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_owl_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_owl_target}
    RUNTIME DESTINATION bin)
endif()
