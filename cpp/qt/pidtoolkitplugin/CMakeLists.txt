include(QmlPlugin)

set(gos_pid_toolkit_ui_cpp_plugin_target pidtoolkitplugin)

list(APPEND gos_pid_toolkit_ui_cpp_plugin_source
  "plugin.h"
  "plugin.cpp")

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Quick
  Core
  Qml)

FindQmlPluginDump()
FindQtInstallQml()

qt5_add_resources(gos_pid_toolkit_ui_cpp_plugin_resources
  ${CMAKE_CURRENT_LIST_DIR}/pidtoolkitplugin.qrc)

list(APPEND gos_pid_toolkit_ui_cpp_plugin_source
  ${gos_pid_toolkit_ui_cpp_plugin_resources})

list(APPEND gos_pid_toolkit_ui_cpp_plugin_include
# ${gos_pid_toolkit_cpp_include}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Qml_INCLUDE_DIRS}
# ${Boost_INCLUDE_DIRS}
)

list(APPEND gos_pid_toolkit_ui_cpp_plugin_libraries
  Qt5::QuickControls2
  Qt5::Quick  
  Qt5::Core
  Qt5::Qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

add_library(${gos_pid_toolkit_ui_cpp_plugin_target} SHARED
  ${gos_pid_toolkit_ui_cpp_plugin_source})

#list(APPEND gos_pid_toolkit_ui_cpp_plugin_include ${Boost_INCLUDE_DIRS})

if(CLANG_TIDY_EXE)
  set_target_properties(${gos_pid_toolkit_ui_cpp_plugin_target} PROPERTIES
    CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
endif()

target_include_directories(${gos_pid_toolkit_ui_cpp_plugin_target} PUBLIC
  ${gos_pid_toolkit_ui_cpp_plugin_include})

if (gos_pid_toolkit_ui_cpp_plugin_definitions)
  target_compile_definitions(${gos_pid_toolkit_ui_cpp_plugin_target} PRIVATE
    ${gos_pid_toolkit_ui_cpp_plugin_definitions})
endif ()

target_compile_features(${gos_pid_toolkit_ui_cpp_plugin_target} PRIVATE
  cxx_auto_type
  cxx_generic_lambdas)

target_link_libraries(${gos_pid_toolkit_ui_cpp_plugin_target} PRIVATE
  ${gos_pid_toolkit_ui_cpp_plugin_libraries})

if (gos_pid_toolkit_ui_cpp_plugin_public_header)
  set_target_properties(${gos_pid_toolkit_ui_cpp_plugin_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_pid_toolkit_ui_cpp_plugin_public_header}")
endif ()

set(gos_pid_toolkit_ui_cpp_plugin_uri "Pid.Toolkit")
set(gos_pid_toolkit_ui_cpp_plugin_version "1.0")
set(gos_pid_toolkit_ui_cpp_plugin_binary_dir ${CMAKE_BINARY_DIR})



set(GENERATE_QMLTYPES_COMMAND
  ${QMLPLUGINDUMP_BIN} -nonrelocatable ${gos_pid_toolkit_ui_cpp_plugin_uri} ${gos_pid_toolkit_ui_cpp_plugin_version} ${gos_pid_toolkit_ui_cpp_plugin_binary_dir} > ${CMAKE_CURRENT_BINARY_DIR}/plugin.qmltypes)

add_custom_command(
    TARGET ${gos_pid_toolkit_ui_cpp_plugin_target}
    POST_BUILD
    COMMAND ${GENERATE_QMLTYPES_COMMAND}
    COMMENT "Generating plugin.qmltypes"
)

install(TARGETS ${gos_pid_toolkit_ui_cpp_plugin_target}
  RUNTIME DESTINATION bin/ui/Pid/Toolkit
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/cpp/gos/pid/toolkit/ui
  ARCHIVE DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir
  DESTINATION bin/ui/Pid/Toolkit)
