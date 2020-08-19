cmake_minimum_required(VERSION 3.12)

project(QtExperimental VERSION 1.0.0.0
  DESCRIPTION "GOS Qt Experimental"
  LANGUAGES CXX)

option(GOS_QT_EXPERIMENTAL_BUILD_MY_EXAMPLE "Build the My Example" OFF)
option(GOS_QT_EXPERIMENTAL_BUILD_QT_MODULE "Build the Qt 514 and GOS Module" OFF)
option(GOS_QT_EXPERIMENTAL_BUILD_QT_SANDBOX "Build the Qt Sandbox" OFF)

if (MSVC)
  option(GOS_DEPLOY_PDB_FILES "Add installation for the PDB files" OFF)
endif ()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/cmake-qmlplugin")

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Charts
  Quick
  Core
  Qml)

if (GOS_QT_EXPERIMENTAL_BUILD_MY_EXAMPLE)

  set(gos_my_plugin_target myplugin)
  set(gos_my_plugin_dir ${CMAKE_CURRENT_SOURCE_DIR}/MyPlugin)
  
  set(gos_my_example_target myexample)

  qt5_add_resources(APP_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/MyExample/qml/app.qrc)

  list(APPEND gos_my_example_source
    "MyExample/main.cpp"
    ${APP_RESOURCES})

  list(APPEND gos_my_example_include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${gos_my_plugin_dir})

  list(APPEND gos_my_example_libraries
    Qt5::Quick
    Qt5::Core
    Qt5::Qml
    ${gos_my_plugin_target})

  add_executable(${gos_my_example_target}
    ${gos_my_example_source})

  target_include_directories(${gos_my_example_target} PRIVATE
    ${gos_my_example_include})

  target_link_libraries(${gos_my_example_target} PRIVATE
    ${gos_my_example_libraries})

  target_compile_features(${gos_my_example_target} PRIVATE
    cxx_auto_type
    cxx_generic_lambdas)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${gos_my_example_target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic)
  endif ()

  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/MyPlugin)

  install(TARGETS ${gos_my_example_target} DESTINATION bin)

  if (GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_my_example_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif ()

endif ()

if (GOS_QT_EXPERIMENTAL_BUILD_QT_MODULE)

  set(gos_qt_gos_module_target qtgosmodule)
  set(gos_qt_gos_module_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/QtGosModule/GosModule/Item)

  set(gos_qt_experimental_target qtexperimental)

  qt5_add_resources(QT_514_RESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Qt514/qml/resources.qrc)

  list(APPEND gos_qt_experimental_source
    ${QT_514_RESOURCES}
    "Qt514/qtexperimental.cpp"
    "Qt514/configuration.cpp"
    "Qt514/configuration.h"
    "Qt514/backend.cpp"
    "Qt514/backend.h"
    "Qt514/items.cpp"
    "Qt514/items.h")

  list(APPEND gos_qt_experimental_include
    "${CMAKE_CURRENT_SOURCE_DIR}/Qt514"
    ${gos_qt_gos_module_dir})

  list(APPEND gos_qt_experimental_libraries
    ${gos_qt_gos_module_target}
    Qt5::QuickControls2
    Qt5::Charts
    Qt5::Quick  
    Qt5::Core
    Qt5::Qml)

  add_executable(${gos_qt_experimental_target}
    ${gos_qt_experimental_source})

  target_include_directories(${gos_qt_experimental_target} PRIVATE
    ${gos_qt_experimental_include})

  target_link_libraries(${gos_qt_experimental_target} PRIVATE
    ${gos_qt_experimental_libraries})

  target_compile_features(${gos_qt_experimental_target} PRIVATE
    cxx_auto_type
    cxx_generic_lambdas)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(${gos_qt_experimental_target} PRIVATE
      -Wall
      -Wextra
      -Wpedantic)
  endif ()

  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/QtGosModule)

  install(TARGETS ${gos_qt_experimental_target} DESTINATION bin)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.ini DESTINATION bin)

  if (GOS_DEPLOY_PDB_FILES)
    install(FILES "$<TARGET_PDB_FILE:${gos_qt_experimental_target}>"
      DESTINATION pdb
      COMPONENT pdb)
  endif ()

endif ()

#add_subdirectory(trycmakeqmlplugin)
add_subdirectory(pidtoolkitplugin)

if (GOS_QT_EXPERIMENTAL_BUILD_QT_SANDBOX)
  #add_subdirectory(QtPlugin)
  add_subdirectory(QtSandbox)

endif ()

message(STATUS "")
message(STATUS "GOS Qt Experimental BUILD SUMMARY")
message(STATUS "  CMAKE_SYSTEM_NAME    : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_MODULE_PATH    : ${CMAKE_MODULE_PATH}")
message(STATUS "  C++ Compiler ID      : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "")
