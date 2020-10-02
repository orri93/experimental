#include <stdio.h>

#include <gos/development.h>

void gos_development_press_any_key() {
  int c;
  printf("Press any key to finish");
  c = getchar();
}
