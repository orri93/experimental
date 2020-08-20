set(gos_experimental_c_tahuspb_target ${gos_spb_experimental_tahu_library})

list(APPEND gos_experimental_c_tahuspb_source
  "${gos_spb_experimental_tahu_client_c_source_dir}/tahu.c")

list(APPEND gos_experimental_c_tahuspb_include
  ${gos_spb_experimental_include})

#if (WIN32)
#  list(APPEND gos_experimental_c_tahuspb_libraries ws2_32)
#endif ()

#list(APPEND gos_experimental_c_tahuspb_definitions
#  NOMINMAX)

add_library(${gos_experimental_c_tahuspb_target}
  ${gos_experimental_c_tahuspb_source})

if (gos_experimental_c_tahuspb_definitions)
  target_compile_definitions(${gos_experimental_c_tahuspb_target} PUBLIC
    ${gos_experimental_c_tahuspb_definitions})
endif ()

target_include_directories(${gos_experimental_c_tahuspb_target} PUBLIC
  ${gos_experimental_c_tahuspb_include})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_tahuspb_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_tahuspb_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
