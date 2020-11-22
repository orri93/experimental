#include <string.h>
#include <stdio.h>

#include <modules/snake/background_actor.h>
#include <modules/snake/macros.h>
#include <modules/snake/sprite.h>
#include <modules/snake/actor.h>

void background_render_handler(actor *a) {
  background_actor *bg = (background_actor *)a;

  sprite_render(&(bg->background_sprite));
  SDL_Rect dest = {.x = 0, .y = 0, .w = ctx.grid_width * TILE_DIMENSION, .h = STATUS_BAR_HEIGHT * TILE_DIMENSION};
  SDL_RenderCopy (ctx.renderer, bg->status_bar, NULL, &dest);

  SDL_QueryTexture(bg->status_text, NULL, NULL, &dest.w, &dest.h);
  dest.x = 80;
  dest.y = 20;
  SDL_RenderCopy (ctx.renderer, bg->status_text, NULL, &dest);
}

background_actor *background_actor_init(const char* fontdir, background_actor *bg) {
  const char* psep = SNAKE_PATH_SEP;
  char fname[FILENAME_MAX];
  size_t lpos;

  actor_init(&bg->a, background_render_handler, NULL);
  sprite_init(
    &bg->background_sprite,
    ctx.grid_width * TILE_DIMENSION,
    ctx.grid_height * TILE_DIMENSION,
    &ctx.sand_decal);
  bg->background_sprite.r[0] = 0;
  bg->background_sprite.r[1] = STATUS_BAR_HEIGHT * TILE_DIMENSION;

  bg->status_bar = ctx.textures[STATUS_BAR_TEXTURE]; // SDL_CreateTextureFromSurface(ctx.renderer, status_surface);

  if (fontdir != NULL) {
    lpos = strlen(fontdir);
    strncpy(fname, fontdir, lpos + 1);
    if (lpos > 0 && fontdir[lpos - 1] != psep[0]) {
      strcat(fname, psep);
    }
    strcat(fname, STATUS_TEXT_FONT);
    bg->font = TTF_OpenFont(fname, STATUS_TEXT_SIZE);
  } else {
    bg->font = TTF_OpenFont(STATUS_TEXT_FONT, STATUS_TEXT_SIZE);
  }
  background_actor_update_scoreboard(bg);

  return bg;
}

background_actor *background_actor_update_scoreboard(background_actor *bg) {
	SDL_Color fg={93,93,93,255};
  char msg[201];
  snprintf(msg, 201, "score: %02u", ctx.score);
  SDL_Surface *status_text_surface = TTF_RenderText_Blended(bg->font, msg, fg);
  bg->status_text = SDL_CreateTextureFromSurface(ctx.renderer, status_text_surface);
  SDL_FreeSurface(status_text_surface);

  return bg;
}
