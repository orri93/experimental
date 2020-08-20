set(gos_experimental_c_sparkplugb_target sparkplugb)

list(APPEND gos_experimental_c_sparkplugb_source
  "sparkplugb.c")

list(APPEND gos_experimental_c_sparkplugb_include
  ${gos_spb_experimental_include})

list(APPEND gos_experimental_c_sparkplugb_libraries
  ${gos_spb_experimental_nanopb_library}
  ${gos_spb_experimental_tahu_library})

#if (WIN32)
#  list(APPEND gos_experimental_c_sparkplugb_libraries ws2_32)
#endif ()

#list(APPEND gos_experimental_c_sparkplugb_definitions
#  NOMINMAX)

add_executable(${gos_experimental_c_sparkplugb_target}
  ${gos_experimental_c_sparkplugb_source})

if (gos_experimental_c_sparkplugb_definitions)
  target_compile_definitions(${gos_experimental_c_sparkplugb_target} PUBLIC
    ${gos_experimental_c_sparkplugb_definitions})
endif ()

target_include_directories(${gos_experimental_c_sparkplugb_target} PUBLIC
  ${gos_experimental_c_sparkplugb_include})

target_link_libraries(${gos_experimental_c_sparkplugb_target}
  ${gos_experimental_c_sparkplugb_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_sparkplugb_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_sparkplugb_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
