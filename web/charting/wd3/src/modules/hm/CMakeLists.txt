if(SDL_DESKTOP_TARGET)
  set(wd3_experiment_c_modules_hm_target hm)

  find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_c_modules_hm_source
    hmdemo.c
    hmm.c
    hm.c)

  list(APPEND wd3_experiment_c_modules_hm_include
    ${SDL_INCLUDE_DIR}
    ${gosc_include_directories}
    ${wd3_experiment_c_include})

  list(APPEND wd3_experiment_c_modules_hm_libraries
    ${gosc_target}
    ${SDL_LIBRARY}
    ${SDLMAIN_LIBRARY})

  if (MSVC)
    list(APPEND wd3_experiment_c_modules_hm_definitions
      HAVE_M_PI)
  endif (MSVC)

  if (USE_SDL_MAIN)
    list(APPEND wd3_experiment_c_modules_hm_definitions
      EFD_USE_SDL_MAIN)
  endif (USE_SDL_MAIN)

  add_executable(${wd3_experiment_c_modules_hm_target}
    ${wd3_experiment_c_modules_hm_source})

  if (wd3_experiment_c_modules_hm_definitions)
    target_compile_definitions(${wd3_experiment_c_modules_hm_target} PUBLIC
      ${wd3_experiment_c_modules_hm_definitions})
  endif ()

  target_include_directories(${wd3_experiment_c_modules_hm_target} PUBLIC
    ${wd3_experiment_c_modules_hm_include})

  target_link_libraries(${wd3_experiment_c_modules_hm_target}
    ${wd3_experiment_c_modules_hm_libraries})

  if(DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${wd3_experiment_c_modules_hm_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif()

  install(TARGETS ${wd3_experiment_c_modules_hm_target}
    RUNTIME DESTINATION bin)
endif()

if(BUILD_TESTS)
  find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_c_modules_hm_testable_source
    hm.c)

  list(APPEND wd3_experiment_c_modules_hm_testable_include
    ${gosc_include_directories}
    ${wd3_experiment_c_include})

  if (SDL_INCLUDE_DIR)
    message(STATUS "  SDL Include detected      : ${SDL_INCLUDE_DIR}")
    list(APPEND wd3_experiment_c_modules_hm_testable_include
      "${SDL_INCLUDE_DIR}")
  elseif (EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR)
    message(STATUS "  EMSDK Include detected    : ${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}")
    list(APPEND wd3_experiment_c_modules_hm_testable_include
      "${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}/SDL")
    #list(APPEND wd3_experiment_c_modules_hm_testable_definitions
    #  HAVE_STDINT_H)
  endif ()

  add_library(${wd3_experiment_c_modules_hm_testable_target}
    ${wd3_experiment_c_modules_hm_testable_source})

  if (wd3_experiment_c_modules_hm_testable_definitions)
    target_compile_definitions(${wd3_experiment_c_modules_hm_testable_target}
      PUBLIC ${wd3_experiment_c_modules_hm_testable_definitions})
  endif ()

  target_include_directories(${wd3_experiment_c_modules_hm_testable_target}
    PUBLIC ${wd3_experiment_c_modules_hm_testable_include})
endif()
