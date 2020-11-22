#include <stdlib.h>

#include <modules/snake/apple_actor.h>
#include <modules/snake/sprite.h>
#include <modules/snake/engine.h>
#include <modules/snake/actor.h>

void apple_render_handler(actor *a) {
  apple_actor *ap = (apple_actor *)a;
  sprite_render(&ap->sprite);
}

apple_actor *apple_actor_init(apple_actor *ap) {
  actor_init(&ap->a, apple_render_handler, NULL);
  sprite_init(
    &ap->sprite,
    TILE_DIMENSION,
    TILE_DIMENSION,
    &ctx.sprites_decals[APPLE]);
  int grid_coords[] = {2, 2};
  engine_get_pixel_coord(grid_coords, ap->sprite.r);
  return ap;
}

void select_random_gridpoint(int *i) {
  i[0] = rand() % ctx.grid_width;
  i[1] = rand() % ctx.grid_height;
}

void apple_replace(apple_actor *ap) {
  int i[2];
  bool placed_apple = false;

  while (!placed_apple) {
    select_random_gridpoint(i);

    if (!ctx.occupied_gridpoints[i[0] + ctx.grid_width * i[1]]) {
      engine_get_pixel_coord(i, ap->sprite.r);
      placed_apple = true;
    }
  }
}

