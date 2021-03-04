if(SDL_DESKTOP_TARGET)
  set(wd3_experiment_c_modules_demo_target demo)

  find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_c_modules_demo_source
    demo.c)

  list(APPEND wd3_experiment_c_modules_demo_include
    ${SDL_INCLUDE_DIR}
    ${gosc_include_directories}
    ${wd3_experiment_c_include})

  list(APPEND wd3_experiment_c_modules_demo_libraries
    ${gosc_target}
    ${SDL_LIBRARY}
    ${SDLMAIN_LIBRARY})

  if (MSVC)
    list(APPEND wd3_experiment_c_modules_demo_definitions
      HAVE_M_PI)
  endif (MSVC)

  if (USE_SDL_MAIN)
    list(APPEND wd3_experiment_c_modules_demo_definitions
      WD3_USE_SDL_MAIN)
  endif (USE_SDL_MAIN)

  add_executable(${wd3_experiment_c_modules_demo_target}
    ${wd3_experiment_c_modules_demo_source})

  if (wd3_experiment_c_modules_demo_definitions)
    target_compile_definitions(${wd3_experiment_c_modules_demo_target} PUBLIC
      ${wd3_experiment_c_modules_demo_definitions})
  endif ()

  target_include_directories(${wd3_experiment_c_modules_demo_target} PUBLIC
    ${wd3_experiment_c_modules_demo_include})

  target_link_libraries(${wd3_experiment_c_modules_demo_target}
    ${wd3_experiment_c_modules_demo_libraries})

  if(DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${wd3_experiment_c_modules_demo_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${wd3_experiment_c_modules_demo_target}
    RUNTIME DESTINATION bin)
endif()
