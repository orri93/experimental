set(opencv_exp_face_target face)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED core imgcodecs objdetect videoio)

list(APPEND opencv_exp_face_source
  controller.cpp
  detection.cpp
  controller.h
  detection.h
  select.cpp
  viewer.cpp
  select.h
  viewer.h
  size.cpp
  face.cpp
  lock.cpp
  size.h)

list(APPEND opencv_exp_face_include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${opencv_exp_include})

list(APPEND opencv_exp_face_libraries
  Qt6::Widgets
  ${OpenCV_LIBS}
  ${opencv_exp_cv_target})

# if (MSVC)
#   list(APPEND opencv_exp_face_definitions
#     HAVE_M_PI)
# endif (MSVC)

add_executable(${opencv_exp_face_target}
  ${opencv_exp_face_source})

if (opencv_exp_face_definitions)
  target_compile_definitions(${opencv_exp_face_target} PUBLIC
    ${opencv_exp_face_definitions})
endif ()

if (opencv_exp_face_include)
  target_include_directories(${opencv_exp_face_target} PUBLIC
    ${opencv_exp_face_include})
endif ()

if (opencv_exp_face_libraries)
  target_link_libraries(${opencv_exp_face_target} PRIVATE
    ${opencv_exp_face_libraries})
endif ()

if(DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${opencv_exp_face_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${opencv_exp_face_target}
  RUNTIME DESTINATION bin)

if (WIN32)
  get_target_property(OPEN_CV_IMPORTED_CONFIGURATIONS opencv_world IMPORTED_CONFIGURATIONS)
  get_target_property(OPEN_CV_IMPORTED_LOCATION_RELEASE opencv_world IMPORTED_LOCATION_RELEASE)
  get_target_property(OPEN_CV_IMPORTED_LOCATION_DEBUG opencv_world IMPORTED_LOCATION_DEBUG)

  add_custom_command(
    TARGET ${opencv_exp_face_target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/windeployqt_stuff
    COMMAND $ENV{QTDIR}/bin/windeployqt.exe --compiler-runtime --dir ${CMAKE_BINARY_DIR}/windeployqt_stuff $<TARGET_FILE:${opencv_exp_face_target}>)
  
  install(
    DIRECTORY ${CMAKE_BINARY_DIR}/windeployqt_stuff/
    DESTINATION ${PROJECT_NAME}
    COMPONENT face_installer)
endif (WIN32)

message(STATUS "")
message(STATUS "${PROJECT_DESCRIPTION} ${PROJECT_VERSION} ${opencv_exp_face_target} BUILD SUMMARY")

message(STATUS "  OpenCV Include            : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "  OpenCV Libraries          : ${OpenCV_LIBS}")
message(STATUS "  OpenCV Installation       : ${OpenCV_INSTALL_PATH}")
message(STATUS "  OpenCV Components         : ${OpenCV_LIB_COMPONENTS}")
message(STATUS "  OpenCV Configurations     : ${OPEN_CV_IMPORTED_CONFIGURATIONS}")
message(STATUS "  OpenCV Release            : ${OPEN_CV_IMPORTED_LOCATION_RELEASE}")
message(STATUS "  OpenCV Debug              : ${OPEN_CV_IMPORTED_LOCATION_DEBUG}")