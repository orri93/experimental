set(gos_experimental_c_gosoutpp_target gosoutpp)

list(APPEND gos_experimental_c_gosoutpp_source
  gosoutpp.cpp)

list(APPEND gos_experimental_c_gosoutpp_include
  ${gos_experimental_include})

list(APPEND gos_experimental_c_gosoutpp_libraries
  ${gos_c_library_target})

#list(APPEND gos_experimental_c_gosoutpp_definitions
#  NOMINMAX)

add_executable(${gos_experimental_c_gosoutpp_target}
  ${gos_experimental_c_gosoutpp_source})

if (gos_experimental_c_gosoutpp_definitions)
  target_compile_definitions(${gos_experimental_c_gosoutpp_target} PUBLIC
    ${gos_experimental_c_gosoutpp_definitions})
endif ()

target_include_directories(${gos_experimental_c_gosoutpp_target} PUBLIC
  ${gos_experimental_c_gosoutpp_include})

target_link_libraries(${gos_experimental_c_gosoutpp_target}
  ${gos_experimental_c_gosoutpp_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_gosoutpp_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_gosoutpp_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
