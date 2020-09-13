set(gos_experimental_c_gosout_target gosout)

list(APPEND gos_experimental_c_gosout_source
  gosout.c)

list(APPEND gos_experimental_c_gosout_include
  ${gos_experimental_include})

list(APPEND gos_experimental_c_gosout_libraries
  ${gos_c_drilling_library_target}
  ${gos_c_library_target})

#list(APPEND gos_experimental_c_gosout_definitions
#  NOMINMAX)

add_executable(${gos_experimental_c_gosout_target}
  ${gos_experimental_c_gosout_source})

if (gos_experimental_c_gosout_definitions)
  target_compile_definitions(${gos_experimental_c_gosout_target} PUBLIC
    ${gos_experimental_c_gosout_definitions})
endif ()

target_include_directories(${gos_experimental_c_gosout_target} PUBLIC
  ${gos_experimental_c_gosout_include})

target_link_libraries(${gos_experimental_c_gosout_target}
  ${gos_experimental_c_gosout_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_gosout_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_gosout_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
