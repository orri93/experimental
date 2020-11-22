#ifndef SPRITE_H
#define SPRITE_H

#include <modules/snake/types.h>

sprite *sprite_init(sprite *sp, int w, int h, decal *d);

void sprite_destroy(sprite *sp);

void sprite_render(sprite *sp);

void sprite_set_decal(sprite *sp, decal *d);

#endif
