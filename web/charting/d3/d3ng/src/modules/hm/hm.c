#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <gos/data.h>
#include <gos/draw.h>
#include <gos/chart.h>
#include <gos/color.h>

#include <modules/hm.h>

#define GOS_D3NG_MD_HM_MESSAGE_SIZE 1024

#define GOS_D3NG_MD_HM_DEMO_WIDTH 600
#define GOS_D3NG_MD_HM_DEMO_HEIGHT 400

