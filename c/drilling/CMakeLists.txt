list(APPEND gos_c_drilling_library_source
  density.c)

#list(APPEND gos_c_drilling_library_definitions)

list(APPEND gos_c_drilling_library_include
  ${gos_experimental_include})

add_library(${gos_c_drilling_library_target}
  ${gos_c_drilling_library_source})

if (gos_c_drilling_library_definitions)
  target_compile_definitions(${gos_c_drilling_library_target} PUBLIC
    ${gos_c_drilling_library_definitions})
endif ()

target_include_directories(${gos_c_drilling_library_target} PUBLIC
  ${gos_c_drilling_library_include})

if (gos_c_drilling_library_public_header)
  set_target_properties(${gos_c_drilling_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_c_drilling_library_public_header}")
endif ()

install(TARGETS ${gos_c_drilling_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/gos/drilling
  ARCHIVE DESTINATION lib)
