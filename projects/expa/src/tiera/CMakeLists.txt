list(APPEND gos_tier_a_library_source
  tiera.c)

#list(APPEND gos_tier_a_library_definitions)

list(APPEND gos_tier_a_library_include
  ${gosc_include_directories}
  ${gos_expa_include})

add_library(${gos_tier_a_library_target}
  ${gos_tier_a_library_source})

if (gos_tier_a_library_definitions)
  target_compile_definitions(${gos_tier_a_library_target} PUBLIC
    ${gos_tier_a_library_definitions})
endif ()

target_include_directories(${gos_tier_a_library_target} PUBLIC
  ${gos_tier_a_library_include})

if (gos_tier_a_library_public_header)
  set_target_properties(${gos_tier_a_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_tier_a_library_public_header}")
endif ()

install(TARGETS ${gos_tier_a_library_target}
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/tiera
  ARCHIVE DESTINATION lib)
