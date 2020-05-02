cmake_minimum_required(VERSION 3.12)

project(QtExperimental VERSION 1.0.0.0
  DESCRIPTION "GOS Qt Experimental"
  LANGUAGES CXX)

set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(gos_qt_experimental_target qtexperimental)

list(APPEND gos_qt_experimental_source
  "qtexperimental.cpp"
  "configuration.cpp"
  "configuration.h"
  "resources.qrc"
  "backend.cpp"
  "backend.h")

list(APPEND gos_qt_experimental_include
  ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND gos_qt_experimental_libraries
  Qt5::QuickControls2
  Qt5::Charts
  Qt5::Quick  
  Qt5::Core
  Qt5::Qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Charts
  Quick
  Core
  Qml)

add_executable(${gos_qt_experimental_target}
  ${gos_qt_experimental_source})

target_include_directories(${gos_qt_experimental_target} PRIVATE
  ${gos_qt_experimental_include})

target_link_libraries(${gos_qt_experimental_target}
  ${gos_qt_experimental_libraries})

add_custom_command(TARGET ${gos_qt_experimental_target} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/Configuration.ini
    $<TARGET_FILE_DIR:${gos_qt_experimental_target}>)

install(TARGETS ${gos_qt_experimental_target}
  LIBRARY DESTINATION bin/ui
  ARCHIVE DESTINATION bin/ui)

message(STATUS "")
message(STATUS "GOS Qt Experimental BUILD SUMMARY")
message(STATUS "  CMAKE_SYSTEM_NAME    : ${CMAKE_SYSTEM_NAME}")
message(STATUS "  CMAKE_GENERATOR      : ${CMAKE_GENERATOR}")
message(STATUS "  CMAKE_MODULE_PATH    : ${CMAKE_MODULE_PATH}")
message(STATUS "  C++ Compiler ID      : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type           : ${CMAKE_BUILD_TYPE}")
message(STATUS "")
