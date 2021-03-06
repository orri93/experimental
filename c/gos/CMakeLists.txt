list(APPEND gos_c_library_source
  interpolate.c
  geometry.c
  perlin.c
  signal.c
  color.c
  noise.c)

list(APPEND gos_c_library_definitions
  GOS_PERLIN_FLOAT
  GOS_PERLIN_DOUBLE
  GOS_PERLIN_LERP_FUNCTION
  _USE_MATH_DEFINES
# _CRT_SECURE_NO_WARNINGS
)

list(APPEND gos_c_library_include
  ${gos_experimental_include})

add_library(${gos_c_library_target}
  ${gos_c_library_source})

if (gos_c_library_definitions)
  target_compile_definitions(${gos_c_library_target} PUBLIC
    ${gos_c_library_definitions})
endif ()

target_include_directories(${gos_c_library_target} PUBLIC
  ${gos_c_library_include})

if (gos_c_library_public_header)
  set_target_properties(${gos_c_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_c_library_public_header}")
endif ()

install(TARGETS ${gos_c_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/gos
  ARCHIVE DESTINATION lib)
