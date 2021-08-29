cmake_minimum_required(VERSION 3.8.2)

project(NeuralNetworkExperiments
  VERSION 1.0.0
  DESCRIPTION "Neural Network Experiments"
  LANGUAGES C)

# Options
option(DEPLOY_PDB_FILES "Deploy PDB files" OFF)

set(nn_genann_dir ${CMAKE_CURRENT_SOURCE_DIR}/extern/genann)
set(nn_genann_src ${nn_genann_dir}/genann.c)

add_subdirectory(src)

message(STATUS "")
message(STATUS "${PROJECT_DESCRIPTION} ${PROJECT_VERSION} BUILD SUMMARY")
message(STATUS "  Install prefix            : ${CMAKE_INSTALL_PREFIX}")

message(STATUS "  CMAKE_GENERATOR           : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_SOURCE_DIR          : ${CMAKE_SOURCE_DIR}")
message(STATUS "  CMAKE_CURRENT_SOURCE_DIR  : ${CMAKE_CURRENT_SOURCE_DIR}")

message(STATUS "  C   Compiler ID           : ${CMAKE_C_COMPILER_ID}")
message(STATUS "  C   Compiler Version      : ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "  C   Compiler flags        : ${CMAKE_C_FLAGS}")
