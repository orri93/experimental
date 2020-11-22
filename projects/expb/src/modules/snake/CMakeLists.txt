set(gos_expb_modules_snake_target snake)

list(APPEND gos_expb_modules_snake_source
  "background_actor.c"
  "input_processor.c"
  "apple_actor.c"
  "snake_actor.c"
  "engine.c"
  "sprite.c"
  "actor.c"
  "decal.c"
  "main.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)
  find_package(SDL2_ttf COMPONENTS REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(PNG COMPONENTS REQUIRED)

  list(APPEND gos_expb_modules_snake_include
    ${gos_expb_include}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL_TTF_INCLUDE_DIRS})

  list(APPEND gos_expb_modules_snake_libraries
    ${SDL2_IMAGE_LIBRARIES}
    SDL2_ttf::SDL2_ttf
    SDL2::SDL2main
    SDL2::SDL2
    PNG::PNG)

  add_executable(${gos_expb_modules_snake_target}
    ${gos_expb_modules_snake_source})

  if (gos_expb_modules_snake_definitions)
    target_compile_definitions(${gos_expb_modules_snake_target} PUBLIC
      ${gos_expb_modules_snake_definitions})
  endif ()

  target_include_directories(${gos_expb_modules_snake_target} PUBLIC
    ${gos_expb_modules_snake_include})

  target_link_libraries(${gos_expb_modules_snake_target}
    ${gos_expb_modules_snake_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_snake_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_snake_target}
    RUNTIME DESTINATION bin)
endif()
