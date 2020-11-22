#include <modules/snake/engine.h>
#include <modules/snake/sprite.h>

sprite *sprite_init(sprite *sp, int w, int h, decal *d) {
  sp->w = w;
  sp->h = h;
  sp->d = d;
  sp->r[0] = 0;
  sp->r[1] = 0;
  return sp;
}

void sprite_render(sprite *sp) {
  SDL_Rect dest;
  dest.x = sp->r[0];
  dest.y = sp->r[1];
  dest.w = sp->w;
  dest.h = sp->h;

  SDL_RenderCopy(ctx.renderer, sp->d->texture, &sp->d->rect, &dest);
}

void sprite_set_decal(sprite *sp, decal *d) {
  sp->d = d;
}
