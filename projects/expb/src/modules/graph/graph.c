#include <math.h>

#include <modules/graph.h>

void gos_graph_draw_grid(gos_graph* g, int thickness, int length) {
  int height, width, cx, cy, pitchx, pitchy;

  thickness = abs(thickness);
  length = abs(length);

  height = gos_graph_get_height_from_coordinate(g, 0.0);
  width = gos_graph_get_width_from_coordinate(g, 0.0);

  pitchx = gos_graph_get_width_from_coordinate(g, g->xscale) - width;
  pitchy = gos_graph_get_height_from_coordinate(g, g->yscale) - height;

  for (cx = 0; cx < g->width; cx++) {
    if ((cx - width) % pitchx == 0) {
      for (cy = height - length; cy <= height + length; cy++) {
        gos_graph_print_pixel_by_window(g, cx, cy);
      }
    } else {
      for (cy = height - thickness; cy <= height + thickness; cy++) {
        gos_graph_print_pixel_by_window(g, cx, cy);
      }
    }
  }

  for (cy = 0; cy < g->height; cy++) {
    if (((cy - height) % pitchy) == 0) {
      for (cx = width - length; cx <= width + length; cx++) {
        gos_graph_print_pixel_by_window(g, cx, cy);
      }
    } else {
      for (cx = width - thickness; cx <= width + thickness; cx++) {
        gos_graph_print_pixel_by_window(g, cx, cy);
      }
    }
  }
}

Uint32* gos_graph_get_pixel_from_coordinates(gos_graph* g, float x, float y) {
  int nx = gos_graph_get_width_from_coordinate(g, x);
  int ny = gos_graph_get_height_from_coordinate(g, y);
  return gos_graph_get_pixel_from_window(g, nx, ny);
}

Uint32* gos_graph_get_pixel_from_window(gos_graph* g, int x, int y) {
  int addition;
  Uint32* pixel = NULL;
  if (y >= 0 && y < g->height && x >= 0 && x < g->width) {
    addition = x + y* g->width;
    pixel = ((Uint32*)(g->surface->pixels)) + addition;
  }
  return pixel;
}

int gos_graph_get_width_from_coordinate(gos_graph* graph, float x) {
  return (int)
    ((float)graph->width * (x - graph->xmin) / (graph->xmax - graph->xmin));
}

int gos_graph_get_height_from_coordinate(gos_graph* graph, float y) {
  return (int)
    ((float)graph->height - (float)graph->height * (y - graph->ymin) /
      (graph->ymax - graph->ymin));
}

void gos_graph_init(gos_graph* graph) {
  gos_graph_set_color(graph, 0Xff, 0xff, 0xff);
}

void gos_graph_print_pixel(gos_graph* graph, float x, float y) {
  gos_graph_print_pixel_color(graph, x, y, graph->color);
}

void gos_graph_print_pixel_by_window(gos_graph* graph, int x, int y) {
  gos_graph_print_pixel_by_window_color(graph, x, y, graph->color);
}

void gos_graph_print_pixel_color(gos_graph* g, float x, float y, Uint32 color) {
  Uint32* pixel = gos_graph_get_pixel_from_coordinates(g, x, y);
  if (pixel != NULL) {
    *pixel = color;
  }
}

void gos_graph_print_pixel_by_window_color(gos_graph* g, int x, int y, Uint32 c) {
  Uint32* pixel = gos_graph_get_pixel_from_window(g, x, y);
  if (pixel != NULL) {
    *pixel = c;
  }
}

void gos_graph_clear(gos_graph* graph) {
  SDL_FillRect(graph->surface, NULL, 0x00000000);
}

void gos_graph_set_color(gos_graph* graph, int r, int g, int b) {
  graph->color = GOS_RGB(r, g, b);
}

void gos_graph_shutdown(gos_graph* graph) {

}
