set(gos_experimental_c_tahuspb_target ${gos_spb_experimental_tahu_library})

# Location of the protoc command needs to be in PATH!

find_package(Python REQUIRED)
find_package(Protobuf REQUIRED)

set(gos_experimental_c_tahus_proto_path ${gos_spb_experimental_tahu_dir})
set(gos_experimental_c_tahus_proto_file "${gos_spb_experimental_tahu_dir}/sparkplug_b/sparkplug_b.proto")
set(gos_experimental_c_tahus_proto_output "${CMAKE_CURRENT_BINARY_DIR}/tahu.pb")

set(gos_experimental_c_tahus_nanopb_options "${gos_spb_experimental_tahu_dir}/client_libraries/c/tahu.options")
set(gos_experimental_c_tahus_nanopb_output_dir "${CMAKE_CURRENT_BINARY_DIR}/sparkplug_b")
set(gos_experimental_c_tahus_nanopb_output "${gos_experimental_c_tahus_nanopb_output_dir}/sparkplug_b.pb.c")

add_custom_command(
  OUTPUT ${gos_experimental_c_tahus_proto_output}
  COMMAND ${Protobuf_PROTOC_EXECUTABLE} --proto_path="${gos_experimental_c_tahus_proto_path}" --descriptor_set_out="${gos_experimental_c_tahus_proto_output}" "${gos_experimental_c_tahus_proto_file}"
  DEPENDS ${SOURCE_FILES} ${Protobuf_PROTOC_EXECUTABLE} ${gos_experimental_c_tahus_proto_file}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
  OUTPUT ${gos_experimental_c_tahus_nanopb_output}
  COMMAND ${Python_EXECUTABLE} "${gos_spb_experimental_nanopb_generator_py}" -f "${gos_experimental_c_tahus_nanopb_options}" "${gos_experimental_c_tahus_proto_output}"
  DEPENDS ${gos_experimental_c_tahus_nanopb_options} ${gos_experimental_c_tahus_proto_output}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND gos_experimental_c_tahuspb_source
  ${gos_experimental_c_tahus_nanopb_output}
  tahu.c)

list(APPEND gos_experimental_c_tahuspb_include
  ${gos_experimental_c_tahus_nanopb_output_dir}
  ${gos_spb_experimental_include}
  ${gos_experimental_include}
  ${CMAKE_CURRENT_BINARY_DIR})

#if (WIN32)
#  list(APPEND gos_experimental_c_tahuspb_libraries ws2_32)
#endif ()

#list(APPEND gos_experimental_c_tahuspb_definitions
#  NOMINMAX)

add_library(${gos_experimental_c_tahuspb_target}
  ${gos_experimental_c_tahuspb_source})

if (gos_experimental_c_tahuspb_definitions)
  target_compile_definitions(${gos_experimental_c_tahuspb_target} PUBLIC
    ${gos_experimental_c_tahuspb_definitions})
endif ()

target_include_directories(${gos_experimental_c_tahuspb_target} PUBLIC
  ${gos_experimental_c_tahuspb_include})

if(GOS_DEPLOY_PDB_FILES)
  install(FILES "$<TARGET_PDB_FILE:${gos_experimental_c_tahuspb_target}>"
    DESTINATION pdb
    COMPONENT pdb)
endif()

install(TARGETS ${gos_experimental_c_tahuspb_target}
# CONFIGURATIONS etc
  RUNTIME DESTINATION bin)
