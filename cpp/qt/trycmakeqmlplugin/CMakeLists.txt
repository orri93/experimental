include(QmlPlugin)

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Quick
  Core
  Qml)

add_qmlplugin(trycmqmlplugin
  URI Gos.Trying
  VERSION 1.0
  SOURCES
    plugin.cpp
    plugin.h
QMLFILES
    qml/Style.qml
#    qtquickcontrols2.conf
#BINARY_DIR
#    ${CMAKE_BINARY_DIR}/src/cpp/pidtoolkitui/qml
)

target_include_directories(trycmqmlplugin PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Qt5Core_INCLUDE_DIRS}
  ${Qt5Qml_INCLUDE_DIRS})

target_link_libraries(trycmqmlplugin PRIVATE
  Qt5::QuickControls2
  Qt5::Quick  
  Qt5::Core
  Qt5::Qml)