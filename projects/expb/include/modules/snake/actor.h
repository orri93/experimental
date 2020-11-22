#ifndef ACTOR_H
#define ACTOR_H

#include <modules/snake/types.h>

actor_list *actor_list_add(actor_list *al, actor *a);

actor *actor_init(
  actor *a,
  actor_render_handler render_handler,
  actor_logic_handler logic_handler);

#endif
