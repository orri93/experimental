#ifndef SNAKE_MACROS_H
#define SNAKE_MACROS_H

#ifdef __EMSCRIPTEN__
#define SNAKE_PATH_SEP "/"
#else
#ifdef WIN32
#define SNAKE_PATH_SEP "\\"
#else
#define SNAKE_PATH_SEP "/"
#endif
#endif

#define STATUS_TEXT_FONT "edosz.ttf"

#define INITIAL_SNAKE_SECONDS_PER_UPDATE 0.2F
#define MINIMUM_SNAKE_SECONDS_PER_UPDATE 0.005F
#define SNAKE_SECONDS_PER_UPDATE_INCREMENT 0.01F

#endif

