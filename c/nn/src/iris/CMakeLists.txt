cmake_minimum_required(VERSION 3.8.2)

set(nn_ex_iris_target nniris)

list(APPEND nn_ex_iris_source
  ${nn_genann_src}
  iris.c)

list(APPEND nn_ex_iris_include
  ${nn_genann_dir})

add_executable(${nn_ex_iris_target}
  ${nn_ex_iris_source})

target_include_directories(${nn_ex_iris_target} PUBLIC
  ${nn_ex_iris_include})

if(DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${nn_ex_iris_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${nn_ex_iris_target}
  RUNTIME DESTINATION bin)
