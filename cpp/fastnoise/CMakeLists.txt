list(APPEND gos_fast_noise_library_source
  "${gos_fast_noise_library_directory}/FastNoise.cpp")

#list(APPEND gos_fast_noise_library_definitions
#  _USE_MATH_DEFINES
# _CRT_SECURE_NO_WARNINGS
#)

list(APPEND gos_fast_noise_library_include
  ${gos_experimental_include})

add_library(${gos_fast_noise_library_target}
  ${gos_fast_noise_library_source})

if (gos_fast_noise_library_definitions)
  target_compile_definitions(${gos_fast_noise_library_target} PUBLIC
    ${gos_fast_noise_library_definitions})
endif ()

target_include_directories(${gos_fast_noise_library_target} PUBLIC
  ${gos_fast_noise_library_include})

if (gos_fast_noise_library_public_header)
  set_target_properties(${gos_fast_noise_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_fast_noise_library_public_header}")
endif ()

install(TARGETS ${gos_fast_noise_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/fastnoise
  ARCHIVE DESTINATION lib)
