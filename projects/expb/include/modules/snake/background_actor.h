#ifndef BACKGROUND_ACTOR_H
#define BACKGROUND_ACTOR_H

#include <modules/snake/types.h>

background_actor *background_actor_init(const char* fontdir, background_actor *bg);
background_actor *background_actor_update_scoreboard(background_actor *bg);

#endif

