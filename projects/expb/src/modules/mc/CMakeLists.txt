list(APPEND gos_expb_modules_mc_library_source
  mandel.c)

list(APPEND gos_expb_modules_mc_library_include
  ${gosc_include_directories}
  ${gos_expb_include})

#list(APPEND gos_expb_modules_mc_library_definitions
#  NOMINMAX)

list(APPEND gos_expb_modules_mc_library_libraries
  ${gosc_target})

if (GOS_USE_CLOCK_GETTIME)
  list(APPEND gos_expb_modules_mc_library_definitions
    GOS_CLOCK_GETTIME)
endif ()

if (HAVE_LIB_M)
  list(APPEND gos_expb_modules_mc_library_libraries m)
endif (HAVE_LIB_M)

add_library(${gos_expb_modules_mc_library_target}
  ${gos_expb_modules_mc_library_source})

if (gos_expb_modules_mc_library_definitions)
  target_compile_definitions(${gos_expb_modules_mc_library_target} PUBLIC
    ${gos_expb_modules_mc_library_definitions})
endif ()
target_include_directories(${gos_expb_modules_mc_library_target} PUBLIC
  ${gos_expb_modules_mc_library_include})
set_property(TARGET ${gos_expb_modules_mc_library_target} PROPERTY C_STANDARD 11)
if (gos_expb_modules_mc_library_public_header)
  set_target_properties(${gos_expb_modules_mc_library_target}
    PROPERTIES PUBLIC_HEADER
    "${gos_expb_modules_mc_library_public_header}")
endif ()
