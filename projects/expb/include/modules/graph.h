#ifndef GOS_EXPB_MODULES_GRAPH_H_
#define GOS_EXPB_MODULES_GRAPH_H_

/* Based on https://github.com/tesch1/sdl-grapher/blob/master/sdlgraph.h */

#include <SDL2/SDL.h>

#define GOS_RGB(r, g, b) ((Uint8)r << 16 | (Uint8)g << 8 | (Uint8)b)

typedef struct gos_graph {
  /* Indicates the smallest value the graph window will show on the x-axis. */
  float xmin;
  /* Largest x */
  float xmax;
  /* Indicates the distance between the "landmarks" on the x-axis */
  float xscale;

  /* Same as indicated over, just on the y-axis */
  float ymin;
  float ymax;
  float yscale;

  /* When the SDL window is initialized,
   * these values will decide the size of it in pixels */
  int width;
  int height;

  /* When a pixel is printed to the screen, this variable will be used
   * to determine which color is to be printed on the screen */
  Uint32 color;

  /* This is the main SDL_Surface that is used transparently
   * in most functions below */
  SDL_Surface* surface;
} gos_graph;

/* This function will draw a grid to the SDL_Surface in the graph-struct.
 * The function also takes thickness of grid and length of the "landmarks" on
 * the grid. 0 length/thickness indicates smallest possible
 * e.g. ( 1 pixel width ) */
void gos_graph_draw_grid(gos_graph* g, int thickness, int length);

/* Returns a pointer to a certain pixel by the means of giving
 * it coordinates on the graph */
Uint32* gos_graph_get_pixel_from_coordinates(gos_graph* g, float x, float y);

/* Returns a pointer to a certain pixel by the means of giving it the coorinates
 * on the SDL window itself. e.g ( 0, 0) will always return
 * the top-left pixel */
Uint32* gos_graph_get_pixel_from_window(gos_graph* graph, int x, int y);

/* Returns the x coordinate on the SDL window itself by using
 * a graph coordinate */
int gos_graph_get_width_from_coordinate(gos_graph* graph, float x);

/* Returns the y coordinate */
int gos_graph_get_height_from_coordinate(gos_graph* graph, float y);

/* Initializes the graph. Sets up an SDL window. This function must be executed
 * before any other API functions! The struct it takes as input must be properly
 * filled in before calling this. */
void gos_graph_init(gos_graph* graph);

/* Prints a pixel to the surface in the struct by using graph coordinates */
void gos_graph_print_pixel(gos_graph* graph, float x, float y);

/* Prints a pixel to the surface in the struct by using
 * SDL window coordinates */
void gos_graph_print_pixel_by_window(gos_graph* graph, int x, int y);

/* Identical to print_pixel(). However, this function allows for defining the
 * color of the pixel to be printed directly without using set_color() first.
 * It will not set the global color either. This might be useful when threading.
 * A Uint32 color can be generated with the RGB macro. */
void gos_graph_print_pixel_color(gos_graph* g, float x, float y, Uint32 color);

/* Same as print_pixel_by_window(). Allows for defining a color on the fly. */
void gos_graph_print_pixel_by_window_color(gos_graph* g, int x, int y, Uint32 c);

/* Clears the graph, returning it to the initial black state.
 * (Still need to update_screen to see the result! */
void gos_graph_clear(gos_graph* graph);

 /* Sets the color in the struct given in normal 24-bit RGB format */
void gos_graph_set_color(gos_graph* graph, int r, int g, int b);

/* Shutdown the Graph */
void gos_graph_shutdown(gos_graph* graph);

#endif
