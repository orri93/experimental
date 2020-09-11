#ifndef GOS_LINE_H_
#define GOS_LINE_H_

#define GOS_SLOPE(x1,y1,x2,y2) \
  ( ( (y2) - (y1) ) / ( (x2) - (x1) ) )

#define GOS_Y_INTERCEPT(x1,y1,m) \
  ( (y1) - (m) * (x1) )

#define GOS_LINE_FUN(x,m,b) ( (m) * (x) + (b) )

typedef struct gos_line {
  double m;
  double b;
} gos_line;

#endif
