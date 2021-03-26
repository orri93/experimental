if(SDL_DESKTOP_TARGET)
  set(wd3_experiment_cpp_modules_hm_target hmpp)

  find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_cpp_modules_hm_source
    "statistics.cpp"
    "exception.cpp"
    "gradient.cpp"
    "context.cpp"
    "column.cpp"
    "global.cpp"
    "point.cpp"
    "data.cpp"
    "demo.cpp"
    "time.cpp"
    "hm.cpp")

  list(APPEND wd3_experiment_cpp_modules_hm_include
    ${SDL_INCLUDE_DIR}
    ${goscpp_include_directories}
    ${wd3_experiment_c_include}
    ${wd3_experiment_extern_date_include})

  list(APPEND wd3_experiment_cpp_modules_hm_libraries
    ${goscpp_target}
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

  if (DEVELOPE_EMSCRIPTEN)
    list(APPEND wd3_experiment_cpp_modules_hm_source
      "emscripten.cpp")
    list(APPEND wd3_experiment_cpp_modules_hm_definitions
      WD3_DEVELOPE_EMSCRIPTEN)
  endif (DEVELOPE_EMSCRIPTEN)

  add_executable(${wd3_experiment_cpp_modules_hm_target}
    ${wd3_experiment_cpp_modules_hm_source})

  if (wd3_experiment_cpp_modules_hm_definitions)
    target_compile_definitions(${wd3_experiment_cpp_modules_hm_target} PUBLIC
      ${wd3_experiment_cpp_modules_hm_definitions})
  endif ()

  set_target_properties(${wd3_experiment_cpp_modules_hm_target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

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

if(BUILD_TESTS)
# find_package(SDL REQUIRED)

  list(APPEND wd3_experiment_cpp_modules_hm_testable_source
    "statistics.cpp"
    "exception.cpp"
    "column.cpp"
    "point.cpp"
    "data.cpp"
    "time.cpp")

  list(APPEND wd3_experiment_cpp_modules_hm_testable_include
    ${goscpp_include_directories}
    ${wd3_experiment_c_include}
    ${wd3_experiment_extern_date_include})

  if (MSVC)
    list(APPEND wd3_experiment_cpp_modules_hm_testable_definitions
      _SILENCE_CXX17_UNCAUGHT_EXCEPTION_DEPRECATION_WARNING)
  endif (MSVC)

  if (SDL_INCLUDE_DIR)
    message(STATUS "  SDL Include detected      : ${SDL_INCLUDE_DIR}")
    list(APPEND wd3_experiment_cpp_modules_hm_testable_include
      "${SDL_INCLUDE_DIR}")
  elseif (EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR)
    message(STATUS "  EMSDK Include detected    : ${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}")
    list(APPEND wd3_experiment_cpp_modules_hm_testable_include
      "${EMSDK_UPSTREAM_EMSCRIPTEN_SYSTEM_INCLUDE_DIR}/SDL")
    #list(APPEND wd3_experiment_c_modules_hm_testable_definitions
    #  HAVE_STDINT_H)
  endif ()

  add_library(${wd3_experiment_cpp_modules_hm_testable_target}
    ${wd3_experiment_cpp_modules_hm_testable_source})

  if (wd3_experiment_cpp_modules_hm_testable_definitions)
    target_compile_definitions(${wd3_experiment_cpp_modules_hm_testable_target}
      PUBLIC ${wd3_experiment_cpp_modules_hm_testable_definitions})
  endif ()

  set_target_properties(${wd3_experiment_cpp_modules_hm_testable_target} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

  target_include_directories(${wd3_experiment_cpp_modules_hm_testable_target}
    PUBLIC ${wd3_experiment_cpp_modules_hm_testable_include})
endif()
