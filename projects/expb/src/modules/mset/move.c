#include <stdio.h>

#include <modules/mset.h>
#include <modules/mset/move.h>

void gmset_move_cross_up(gos_mset_context* context, gosmf r) {
  context->mcpoint.y -= r * (context->mrect.b.t - context->mrect.b.f);
  context->isdrawcross = true;
  context->isdraw = true;
}

void gmset_move_cross_down(gos_mset_context* context, gosmf r) {
  context->mcpoint.y += r * (context->mrect.b.t - context->mrect.b.f);
  context->isdrawcross = true;
  context->isdraw = true;
}

void gmset_move_cross_left(gos_mset_context* context, gosmf r) {
  context->mcpoint.x -= r * (context->mrect.a.t - context->mrect.a.f);
  context->isdrawcross = true;
  context->isdraw = true;
}

void gmset_move_cross_right(gos_mset_context* context, gosmf r) {
  context->mcpoint.x += r * (context->mrect.a.t - context->mrect.a.f);
  context->isdrawcross = true;
  context->isdraw = true;
}
