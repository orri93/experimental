cmake_minimum_required(VERSION 3.12)

project(QtSandbox VERSION 1.0.0.0
  DESCRIPTION "GOS Qt Sandbox"
  LANGUAGES CXX)

if (MSVC)
  option(GOS_DEPLOY_PDB_FILES "Add installation for the PDB files" OFF)
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

set(gos_qt_sandbox_target qtsandbox)

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Charts
  Quick
  Core
  Qml)

qt5_add_resources(gos_qt_sandbox_qt_resources
  ${CMAKE_CURRENT_LIST_DIR}/qml/qtsandbox.qrc)

qt5_add_resources(gos_qt_sandbox_qt_assets_resources
  ${CMAKE_CURRENT_LIST_DIR}/assets.qrc)

list(APPEND gos_qt_sandbox_source
  ${gos_qt_sandbox_qt_assets_resources}
  ${gos_qt_sandbox_qt_resources}
  "qtsandbox.cpp"
  "configuration.cpp"
  "configuration.h"
  "restriction.cpp"
  "restriction.h"
  "accuracy.cpp"
  "backend.cpp"
  "format.cpp"
  "accuracy.h"
  "backend.h"
  "range.cpp"
  "format.h"
  "range.h"
  "ui.cpp"
  "ui.h"
)

list(APPEND gos_qt_sandbox_include
  ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND gos_qt_sandbox_libraries
  Qt5::QuickControls2
  Qt5::Charts
  Qt5::Quick
  Qt5::Core
  Qt5::Qml)

add_executable(${gos_qt_sandbox_target}
  ${gos_qt_sandbox_source})

target_include_directories(${gos_qt_sandbox_target} PRIVATE
  ${gos_qt_sandbox_include})

target_link_libraries(${gos_qt_sandbox_target} PRIVATE
  ${gos_qt_sandbox_libraries})

target_compile_features(${gos_qt_sandbox_target} PRIVATE
  cxx_auto_type
  cxx_generic_lambdas)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${gos_qt_sandbox_target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic)
endif ()

#add_custom_command(TARGET ${gos_qt_experimental_target} POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.ini
#    $<TARGET_FILE_DIR:${gos_qt_experimental_target}>)

install(TARGETS ${gos_qt_sandbox_target} DESTINATION bin)
#install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.ini DESTINATION bin)

if (GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_qt_sandbox_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif ()

message(STATUS "")
message(STATUS "GOS Qt Experimental BUILD SUMMARY")
message(STATUS "  CMAKE_SYSTEM_NAME    : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_MODULE_PATH    : ${CMAKE_MODULE_PATH}")
message(STATUS "  C++ Compiler ID      : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "")
