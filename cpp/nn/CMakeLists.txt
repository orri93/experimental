set(gos_experimental_cpp_nn_target gosexpnn)

list(APPEND gos_experimental_cpp_nn_source
  "leukemia.cpp")

list(APPEND gos_experimental_cpp_nn_include
  ${gos_nn_experimental_opennn_include})

list(APPEND gos_experimental_cpp_nn_libraries
  ${gos_nn_experimental_opennn_library})

#if (WIN32)
#  list(APPEND gos_experimental_cpp_nn_libraries ws2_32)
#endif ()

#list(APPEND gos_experimental_cpp_nn_definitions
#  NOMINMAX)

add_executable(${gos_experimental_cpp_nn_target}
  ${gos_experimental_cpp_nn_source})

if (gos_experimental_cpp_nn_definitions)
  target_compile_definitions(${gos_experimental_cpp_nn_target} PUBLIC
    ${gos_experimental_cpp_nn_definitions})
endif ()

target_include_directories(${gos_experimental_cpp_nn_target} PUBLIC
  ${gos_experimental_cpp_nn_include})

target_link_libraries(${gos_experimental_cpp_nn_target}
  ${gos_experimental_cpp_nn_libraries})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_cpp_nn_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_cpp_nn_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
