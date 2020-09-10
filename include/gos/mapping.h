#ifndef GOS_MAPPING_H_
#define GOS_MAPPING_H_

#define GOS_MAP(v, minx, maxx, minout, maxout) \
  ((minoout) + ((((v) - (minx)) * ((maxout) - (minout))) / ((maxx) - (minx))))

#endif
