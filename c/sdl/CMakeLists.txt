set(gos_experimental_c_sdl_target sdlexp)

list(APPEND gos_experimental_c_sdl_source
  "render.c"
  "sdl.c")

list(APPEND gos_experimental_c_sdl_include
  ${gos_experimental_include})

#list(APPEND gos_experimental_c_sdl_libraries)

if (GOS_SDL_WEB_EXPERIMENTAL)
  
else ()
  find_package(SDL2 COMPONENTS REQUIRED)
  list(APPEND gos_experimental_c_sdl_libraries
    ${SDL2_LIBRARIES}
    SDL2::SDL2-static
    SDL2::SDL2main)
endif ()

#if (WIN32)
#  list(APPEND gos_experimental_c_sdl_libraries ws2_32)
#endif ()

#list(APPEND gos_experimental_c_sdl_definitions
#  NOMINMAX)

add_executable(${gos_experimental_c_sdl_target}
  ${gos_experimental_c_sdl_source})

if (gos_experimental_c_sdl_definitions)
  target_compile_definitions(${gos_experimental_c_sdl_target} PUBLIC
    ${gos_experimental_c_sdl_definitions})
endif ()

target_include_directories(${gos_experimental_c_sdl_target} PUBLIC
  ${gos_experimental_c_sdl_include})

target_link_libraries(${gos_experimental_c_sdl_target}
  ${gos_experimental_c_sdl_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_sdl_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_sdl_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
