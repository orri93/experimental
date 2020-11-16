#include <stdio.h>

#include <modules/mset.h>
#include <modules/mset/scale.h>

void gmset_scale_resize(gos_mset_context* context, gosmf r) {
  gosmf wd, hd;
  wd = context->mrect.a.t - context->mrect.a.f;
  hd = context->mrect.b.t - context->mrect.b.f;
  wd *= GOS_MSET_HALF * r;
  hd *= GOS_MSET_HALF * r;
  context->mrect.a.f = context->mcpoint.x - wd;
  context->mrect.a.t = context->mcpoint.x + wd;
  context->mrect.b.f = context->mcpoint.y - hd;
  context->mrect.b.t = context->mcpoint.y + hd;
  context->isresize = true;
  context->isdraw = true;
}
