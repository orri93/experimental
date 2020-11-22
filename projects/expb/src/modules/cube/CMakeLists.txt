set(gos_expb_modules_cube_target cube)

list(APPEND gos_expb_modules_cube_source
  "cube.c")

if(GOS_SDL_DESKTOP_TARGET)

  find_package(OpenGL COMPONENTS REQUIRED)
  find_package(GLUT COMPONENTS REQUIRED)
  list(APPEND gos_expb_modules_cube_definitions
    GOS_USE_FREEGLUT)

  if (WIN32)
    list(APPEND gos_expb_modules_cube_definitions
      FREEGLUT_STATIC)
  endif (WIN32)
  
  list(APPEND gos_expb_modules_cube_include
    ${soil_include_directory}
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR})

  list(APPEND gos_expb_modules_cube_libraries
    ${soil_target}
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES})

  add_executable(${gos_expb_modules_cube_target}
    ${gos_expb_modules_cube_source})

  if (gos_expb_modules_cube_definitions)
    target_compile_definitions(${gos_expb_modules_cube_target} PUBLIC
      ${gos_expb_modules_cube_definitions})
  endif ()

  target_include_directories(${gos_expb_modules_cube_target} PUBLIC
    ${gos_expb_modules_cube_include})

  target_link_libraries(${gos_expb_modules_cube_target}
    ${gos_expb_modules_cube_libraries})

  if(GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_expb_modules_cube_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${gos_expb_modules_cube_target}
    RUNTIME DESTINATION bin)
endif()
