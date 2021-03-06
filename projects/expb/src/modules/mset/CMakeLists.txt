set(gos_expb_modules_mset_target mset)

list(APPEND gos_expb_modules_mset_source
  "texture.c"
  "handle.c"
  "mouse.c"
  "scale.c"
  "items.c"
  "loop.c"
  "move.c"
  "draw.c"
  "mset.c"
  "map.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(SDL2 COMPONENTS REQUIRED)
  find_package(SDL2_ttf COMPONENTS REQUIRED)
  find_package(PNG COMPONENTS REQUIRED)

  message(STATUS "SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")

  list(APPEND gos_expb_modules_mset_include
    ${SDL_TTF_INCLUDE_DIRS}
    ${gos_experimental_include}
    ${gosc_include_directories}
    ${gos_expb_include})

  list(APPEND gos_expb_modules_mset_libraries
    ${gos_expb_modules_mc_library_target}
    ${gosc_target}
    SDL2_ttf::SDL2_ttf
    SDL2::SDL2-static
    SDL2::SDL2main
    PNG::PNG)

  if (MSVC)
    list(APPEND gos_expb_modules_mset_definitions
      HAVE_M_PI)
  endif (MSVC)

  add_executable(${gos_expb_modules_mset_target}
    ${gos_expb_modules_mset_source})

  if (gos_expb_modules_mset_definitions)
    target_compile_definitions(${gos_expb_modules_mset_target} PUBLIC
      ${gos_expb_modules_mset_definitions})
  endif ()

  target_include_directories(${gos_expb_modules_mset_target} PUBLIC
    ${gos_expb_modules_mset_include})

  target_link_libraries(${gos_expb_modules_mset_target}
    ${gos_expb_modules_mset_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_mset_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_mset_target}
    RUNTIME DESTINATION bin)
endif()
