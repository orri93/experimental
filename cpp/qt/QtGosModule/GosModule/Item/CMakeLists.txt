list(APPEND gos_qt_gos_module_source
  "validation/float.cpp"
  "validation/float.h"
  "model/interval.cpp")

list(APPEND gos_qt_gos_module_include
  ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND gos_qt_gos_module_libraries
  Qt5::Quick  
  Qt5::Core
  Qt5::Qml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

qt5_add_resources(GOS_MODULE_RESOURCES
  ${CMAKE_CURRENT_LIST_DIR}/qml/gosmodule.qrc)
list(APPEND gos_qt_gos_module_source
  ${GOS_MODULE_RESOURCES})

find_package(Qt5 COMPONENTS REQUIRED
  Quick
  Core
  Qml)

message(STATUS "${gos_qt_gos_module_target}")
message(STATUS "${gos_qt_gos_module_source}")
message(STATUS "${gos_qt_gos_module_libraries}")
add_library(${gos_qt_gos_module_target} SHARED
  ${gos_qt_gos_module_source})

target_include_directories(${gos_qt_gos_module_target} PRIVATE
  ${gos_qt_gos_module_dir})

target_link_libraries(${gos_qt_gos_module_target} PRIVATE
  ${gos_qt_gos_module_libraries})

target_compile_features(${gos_qt_gos_module_target} PRIVATE
  cxx_auto_type
  cxx_generic_lambdas)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${gos_qt_gos_module_target} PRIVATE
    -Wall
    -Wextra
    -Wpedantic)
endif ()

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
add_custom_command(TARGET ${gos_qt_gos_module_target}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_LIST_DIR}/qmldir
    $<TARGET_FILE_DIR:${gos_qt_gos_module_target}>/qmldir)

install(TARGETS ${gos_qt_gos_module_target}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
