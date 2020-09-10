add_subdirectory(gos)

if (GOS_SDL_EXPERIMENTAL)
  add_subdirectory(sdl)
endif ()

if (GOS_SPB_EXPERIMENTAL)
  add_subdirectory(tahuspb)
  add_subdirectory(sparkplugb)
endif ()
