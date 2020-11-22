#include <modules/snake/snake_actor.h>
#include <modules/snake/apple_actor.h>
#include <modules/snake/background_actor.h>
#include <modules/snake/input_processor.h>
#include <modules/snake/macros.h>
#include <modules/snake/sprite.h>
#include <modules/snake/engine.h>
#include <modules/snake/actor.h>

void snake_render_handler(actor *a) {
  int i;
  snake_actor *sn = (snake_actor *)a;
  for (i = 0; i < sn->num_segments; i++) {
    sprite_render(&(sn->snake_sprites[i]));
  }
}

void snake_direction_to_vector(snake_actor *sn, int *r) {
  switch (sn->direction) {
  case DIRECTION_N:
    r[0] = 0;
    r[1] = -1;
    break;
  case DIRECTION_E:
    r[0] = 1;
    r[1] = 0;
    break;
  case DIRECTION_S:
    r[0] = 0;
    r[1] = 1;
    break;
  case DIRECTION_W:
    r[0] = -1;
    r[1] = 0;
    break;
  }
}

SPRITES_DECALS snake_get_neck_decal(snake_actor *sn, DIRECTION old_direction) {
  switch (sn->direction) {
  case DIRECTION_N:
    switch (old_direction) {
    case DIRECTION_W:
      return BODY_NE;
    case DIRECTION_E:
      return BODY_NW;
    default:
      return BODY_N;
    }
  case DIRECTION_E:
    switch (old_direction) {
    case DIRECTION_N:
      return BODY_SE;
    case DIRECTION_S:
      return BODY_NE;
    default:
      return BODY_E;
    }
  case DIRECTION_S:
    switch (old_direction) {
    case DIRECTION_W:
      return BODY_SE;
    case DIRECTION_E:
      return BODY_SW;
    default:
      return BODY_N;
    }
    return BODY_N;
  case DIRECTION_W:
    switch (old_direction) {
    case DIRECTION_N:
      return BODY_SW;
    case DIRECTION_S:
      return BODY_NW;
    default:
      return BODY_E;
    }
  }
  return BODY_N;
}

SPRITES_DECALS snake_get_head_decal(snake_actor *sn) {
  switch (sn->direction) {
  case DIRECTION_N:
    return HEAD_N;
  case DIRECTION_E:
    return HEAD_E;
  case DIRECTION_S:
    return HEAD_S;
  case DIRECTION_W:
    return HEAD_W;
  }
  return HEAD_N;
}

SPRITES_DECALS snake_get_tail_decal(snake_actor *sn) {
  unsigned int next_to_tail_index = (sn->tail_index + 1) % sn->num_segments;

  if (sn->snake_sprites[next_to_tail_index].r[0] > sn->snake_sprites[sn->tail_index].r[0]) {
    if (sn->snake_sprites[next_to_tail_index].r[0] - sn->snake_sprites[sn->tail_index].r[0] > sn->segment_w) {
      return TAIL_W;
    }
    return TAIL_E;
  }
    
  if (sn->snake_sprites[next_to_tail_index].r[0] < sn->snake_sprites[sn->tail_index].r[0]) {
    if (sn->snake_sprites[sn->tail_index].r[0] - sn->snake_sprites[next_to_tail_index].r[0] > sn->segment_w) {
      return TAIL_E;
    }
    return TAIL_W;
  }

  if (sn->snake_sprites[next_to_tail_index].r[1] > sn->snake_sprites[sn->tail_index].r[1]) {
    if (sn->snake_sprites[next_to_tail_index].r[1] - sn->snake_sprites[sn->tail_index].r[1] > sn->segment_h) {
      return TAIL_N;
    }
    return TAIL_S;
  }

  if (sn->snake_sprites[sn->tail_index].r[1] - sn->snake_sprites[next_to_tail_index].r[1] > sn->segment_h) {
    return TAIL_S;
  }
  return TAIL_N;
}

sprite *snake_get_head_sprite(snake_actor *sn) {
  int head_index = ((sn->tail_index - 1 + sn->num_segments) % sn->num_segments);
  return &sn->snake_sprites[head_index];
}

void snake_apple_collision_detect(snake_actor *sn) {
  sprite *snake_head = snake_get_head_sprite(sn);
  if (
    snake_head->r[0] == ctx.apple_actor.sprite.r[0] &&
    snake_head->r[1] == ctx.apple_actor.sprite.r[1]) {
    apple_replace(&ctx.apple_actor);
    sn->needs_to_grow = true;
    ctx.score += 1;
    background_actor_update_scoreboard(&ctx.background_actor);

    float seconds_per_update = INITIAL_SNAKE_SECONDS_PER_UPDATE -
      (float)ctx.score * SNAKE_SECONDS_PER_UPDATE_INCREMENT;
    if (seconds_per_update < MINIMUM_SNAKE_SECONDS_PER_UPDATE) {
      seconds_per_update = MINIMUM_SNAKE_SECONDS_PER_UPDATE;
    }

    sn->snake_frame_ratio =
      (unsigned int)(((float)ctx.fps) * seconds_per_update);
  }
}

void snake_grow(snake_actor *sn) {
  sn->num_segments += 1;
  sprite_init(
    &(sn->snake_sprites[sn->num_segments - 1]),
    sn->segment_w,
    sn->segment_h,
    &(ctx.sprites_decals[BODY_N]));

  sprite *new_segment = &(sn->snake_sprites[sn->num_segments - 1]);

  int i;
  for (i = sn->num_segments - 1; i > sn->tail_index; i--) {
    sn->snake_sprites[i].r[0] = sn->snake_sprites[i-1].r[0];
    sn->snake_sprites[i].r[1] = sn->snake_sprites[i-1].r[1];
    sprite_set_decal(&sn->snake_sprites[i], sn->snake_sprites[i-1].d);
  }

  unsigned int grid_x = new_segment->r[0] / TILE_DIMENSION;
  unsigned int grid_y = new_segment->r[1] / TILE_DIMENSION - STATUS_BAR_HEIGHT;
  ctx.occupied_gridpoints[grid_x + ctx.grid_width * grid_y] = true;
}

void snake_logic_handler(actor *a) {
  snake_actor * sn = (snake_actor *)a;
  if (ctx.current_frame % sn->snake_frame_ratio == 0) {
    DIRECTION old_direction = sn->direction;

    if (is_state_active(GS_N)) {
      deactivate_state(GS_N);
      if (sn->direction != DIRECTION_S) {
        sn->direction = DIRECTION_N;
      }
    } else if (is_state_active(GS_E)) {
      deactivate_state(GS_E);
      if (sn->direction != DIRECTION_W) {
        sn->direction = DIRECTION_E;
      }
    } else if (is_state_active(GS_S)) {
      deactivate_state(GS_S);
      if (sn->direction != DIRECTION_N) {
        sn->direction = DIRECTION_S;
      }
    } else if (is_state_active(GS_W)) {
      deactivate_state(GS_W);

      if (sn->direction != DIRECTION_E) {
        sn->direction = DIRECTION_W;
      }
    }

    if (sn->needs_to_grow && sn->num_segments < MAX_SNAKE_SEGMENTS) {
      snake_grow(sn);
      sn->needs_to_grow = false;
    }

    sprite *old_head = snake_get_head_sprite(sn);
    SPRITES_DECALS decal = snake_get_neck_decal(sn, old_direction);
    sprite_set_decal(old_head, &(ctx.sprites_decals[decal]));

    int new_head_index = sn->tail_index;
    sprite *new_head = &sn->snake_sprites[new_head_index];
        
    int old_grid_coords[2];
    engine_get_grid_coord(new_head->r, old_grid_coords);

    int snake_direction[2];
    snake_direction_to_vector(sn, snake_direction);

    int new_grid_coords[2];
    engine_get_grid_coord(old_head->r, new_grid_coords);
    new_grid_coords[0] += snake_direction[0];
    new_grid_coords[1] += snake_direction[1];
    engine_apply_boundary_conditions(new_grid_coords);

    engine_get_pixel_coord(new_grid_coords, new_head->r);

    if (ctx.occupied_gridpoints[new_grid_coords[0] + ctx.grid_width * new_grid_coords[1]]) {
      engine_reset();
      return;
    }
    ctx.occupied_gridpoints[old_grid_coords[0] + ctx.grid_width * old_grid_coords[1]] = false;
    ctx.occupied_gridpoints[new_grid_coords[0] + ctx.grid_width * new_grid_coords[1]] = true;

    decal = snake_get_head_decal(sn);
    sprite_set_decal(new_head, &(ctx.sprites_decals[decal]));
        
    sn->tail_index = (sn->tail_index + 1) % sn->num_segments;

    decal = snake_get_tail_decal(sn);
    sprite_set_decal(&sn->snake_sprites[sn->tail_index], &(ctx.sprites_decals[decal]));

    snake_apple_collision_detect(sn);
  }
}

snake_actor *snake_actor_init(snake_actor *sn) {
  sn->num_segments = 3;
  sn->segment_w = TILE_DIMENSION;
  sn->segment_h = TILE_DIMENSION;
  sn->needs_to_grow = false;

  actor_init(&(sn->a), snake_render_handler, snake_logic_handler);
  int i;
  SPRITES_DECALS snake_init_config[] = {TAIL_E, BODY_E, HEAD_E};
  int tail_pos[2];
  int tail_grid_pos[2] = {4,4};
  engine_get_pixel_coord(tail_grid_pos, tail_pos);

  for (i = 0; i < sn->num_segments; i++) {
    sprite_init(
      &(sn->snake_sprites[i]),
      sn->segment_w,
      sn->segment_h,
      &(ctx.sprites_decals[snake_init_config[i]]));
    sn->snake_sprites[i].r[0] = tail_pos[0] + i * sn->segment_w;
    sn->snake_sprites[i].r[1] = tail_pos[1];

    unsigned int grid_x = sn->snake_sprites[i].r[0] / TILE_DIMENSION;
    unsigned int grid_y = sn->snake_sprites[i].r[1] / TILE_DIMENSION - STATUS_BAR_HEIGHT;
    ctx.occupied_gridpoints[grid_x + ctx.grid_width * grid_y] = true;
  }
  sn->direction = DIRECTION_E;

  float seconds_per_update = INITIAL_SNAKE_SECONDS_PER_UPDATE;
  sn->snake_frame_ratio =
    (unsigned int)(((float)ctx.fps) * seconds_per_update);

  sn->tail_index = 0;
  return sn;
}
