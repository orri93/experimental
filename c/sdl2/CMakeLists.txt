set(gos_experimental_c_sdl_2_target sdl2exp)

find_package(SDL2 COMPONENTS REQUIRED)
find_package(SDL2_ttf COMPONENTS REQUIRED)
find_package(PNG COMPONENTS REQUIRED)

list(APPEND gos_experimental_c_sdl_2_source
  "sdl2exp.c"
  "sdl2.c")

list(APPEND gos_experimental_c_sdl_2_include
  ${SDL_TTF_INCLUDE_DIRS}
  ${gos_experimental_include})

list(APPEND gos_experimental_c_sdl_2_libraries
  SDL2_ttf::SDL2_ttf
  SDL2::SDL2-static
  SDL2::SDL2main
  PNG::PNG)

add_executable(${gos_experimental_c_sdl_2_target}
  ${gos_experimental_c_sdl_2_source})

if (gos_experimental_c_sdl_2_definitions)
  target_compile_definitions(${gos_experimental_c_sdl_2_target} PUBLIC
    ${gos_experimental_c_sdl_2_definitions})
endif ()

target_include_directories(${gos_experimental_c_sdl_2_target} PUBLIC
  ${gos_experimental_c_sdl_2_include})

target_link_libraries(${gos_experimental_c_sdl_2_target}
  ${gos_experimental_c_sdl_2_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_sdl_2_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_sdl_2_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
