cmake_minimum_required(VERSION 3.8.2)

project(OpcUaExperiments
  VERSION 1.0.0
  DESCRIPTION "OPC UA Experiments"
  LANGUAGES C)

# Options
option(DEPLOY_PDB_FILES "Deploy PDB files" OFF)

find_package(open62541 REQUIRED)

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
