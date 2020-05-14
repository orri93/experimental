list(APPEND gos_my_plugin_source
  "myquickitem.cpp"
  "myquickitem.h"
  "myplugin.h"
  "myplugin.cpp")

list(APPEND gos_my_plugin_include
  ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND gos_my_plugin_libraries
  Qt5::Quick  
  Qt5::Core
  Qt5::Qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

qt5_add_resources(PLUGIN_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/qml/plugin.qrc)
list(APPEND gos_my_plugin_source ${PLUGIN_RESOURCES})

find_package(Qt5 COMPONENTS REQUIRED
  Quick
  Core
  Qml)

add_library(${gos_my_plugin_target} SHARED
  ${gos_my_plugin_source})

target_include_directories(${gos_my_plugin_target} PRIVATE
  ${gos_my_plugin_dir})

target_link_libraries(${gos_my_plugin_target} PRIVATE
  ${gos_my_plugin_libraries})

target_compile_features(${gos_my_plugin_target} PRIVATE
  cxx_auto_type
  cxx_generic_lambdas)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${gos_my_plugin_target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic)
endif ()

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
add_custom_command(TARGET ${gos_my_plugin_target}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_LIST_DIR}/qmldir
    $<TARGET_FILE_DIR:${gos_my_plugin_target}>/qmldir)

install(TARGETS ${gos_my_plugin_target}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
