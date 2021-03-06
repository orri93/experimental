if(SDL_DESKTOP_TARGET)
  set(wd3_experiment_cpp_modules_hm_target hmpp)

  find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_cpp_modules_hm_source
    exception.cpp
    gradient.cpp
    context.cpp
    column.cpp
    point.cpp
    data.cpp
    demo.cpp
    hm.cpp)

  list(APPEND wd3_experiment_cpp_modules_hm_include
    ${SDL_INCLUDE_DIR}
    ${gosc_include_directories}
    ${wd3_experiment_c_include})

  list(APPEND wd3_experiment_cpp_modules_hm_libraries
    ${gosc_target}
    ${SDL_LIBRARY}
    ${SDLMAIN_LIBRARY})

  if (MSVC)
    list(APPEND wd3_experiment_cpp_modules_hm_definitions
      HAVE_M_PI)
  endif (MSVC)

  if (USE_SDL_MAIN)
    list(APPEND wd3_experiment_cpp_modules_hm_definitions
      WD3_USE_SDL_MAIN)
  endif (USE_SDL_MAIN)

  add_executable(${wd3_experiment_cpp_modules_hm_target}
    ${wd3_experiment_cpp_modules_hm_source})

  if (wd3_experiment_cpp_modules_hm_definitions)
    target_compile_definitions(${wd3_experiment_cpp_modules_hm_target} PUBLIC
      ${wd3_experiment_cpp_modules_hm_definitions})
  endif ()

  target_include_directories(${wd3_experiment_cpp_modules_hm_target} PUBLIC
    ${wd3_experiment_cpp_modules_hm_include})

  target_link_libraries(${wd3_experiment_cpp_modules_hm_target}
    ${wd3_experiment_cpp_modules_hm_libraries})

  if(DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${wd3_experiment_cpp_modules_hm_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${wd3_experiment_cpp_modules_hm_target}
    RUNTIME DESTINATION bin)
endif()
