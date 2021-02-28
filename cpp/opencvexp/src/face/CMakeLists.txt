set(opencv_exp_face_target face)

find_package(Qt5 COMPONENTS REQUIRED
  QuickControls2
  Quick
  Core
  Qml)

find_package(OpenCV REQUIRED core imgcodecs objdetect videoio)

qt5_add_resources(opencv_exp_face_qt_resources
  ${CMAKE_CURRENT_LIST_DIR}/qml/face.qrc)

qt5_add_resources(opencv_exp_face_qt_assets_resources
  ${CMAKE_CURRENT_LIST_DIR}/assets.qrc)

list(APPEND opencv_exp_face_source
  ${opencv_exp_face_qt_assets_resources}
  ${opencv_exp_face_qt_resources}
  face.cpp)

# list(APPEND opencv_exp_face_include )

list(APPEND opencv_exp_face_libraries
  Qt5::QuickControls2
  Qt5::Quick
  Qt5::Core
  Qt5::Qml
  ${OpenCV_LIBS})

message(STATUS "  Face Libraries            : ${opencv_exp_face_libraries}")

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

target_link_libraries(${opencv_exp_face_target}
  ${opencv_exp_face_libraries})

if(DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${opencv_exp_face_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${opencv_exp_face_target}
  RUNTIME DESTINATION bin)
