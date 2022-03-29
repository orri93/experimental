#define TWO_PI 2.0 * M_PI

REAL FUNCTION next_x(x, n, t)
  REAL x, n, t
  next_x = x + n ** (1.5 / n + 1000.0)
  RETURN
END FUNCTION next_x

PROGRAM main
  IMPLICIT NONE
  PRINT '(a, /)', 'command-line options:'
END PROGRAM
