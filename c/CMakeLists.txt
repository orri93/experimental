add_subdirectory(drilling)

add_subdirectory(gos)
add_subdirectory(gosout)

if (GOS_SDL_EXPERIMENTAL)
  add_subdirectory(sdl)
endif ()

if (GOS_SPB_EXPERIMENTAL)
  add_subdirectory(tahuspb)
  add_subdirectory(sparkplugb)
endif ()
