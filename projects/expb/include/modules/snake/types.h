#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include <SDL_ttf.h>

typedef struct engine engine;

typedef enum DIRECTION {
  DIRECTION_N,
  DIRECTION_E,
  DIRECTION_S,
  DIRECTION_W
} DIRECTION;

enum {
  SAND_TEXTURE,
  SPRITES_TEXTURE,
  STATUS_BAR_TEXTURE,
  NUM_TEXTURES
};

typedef enum SPRITES_DECALS {
  TAIL_S,
  TAIL_W,
  HEAD_N,
  HEAD_E,
  BODY_SE,
  BODY_SW,
  BODY_NW,
  BLANK,
  TAIL_E,
  TAIL_N,
  HEAD_W,
  HEAD_S,
  BODY_N,
  BODY_E,
  BODY_NE,
  APPLE,
  NUM_SPRITES_DECALS
} SPRITES_DECALS;

enum {
  MAX_SNAKE_SEGMENTS = 50,
  TILE_DIMENSION = 40,
  STATUS_BAR_HEIGHT = 2, // given in tiles
  STATUS_TEXT_SIZE = 30
};

typedef enum game_state {
  GS_ONE,
  GS_TWO,
  GS_N,
  GS_E,
  GS_S,
  GS_W,
  GS_QUIT,
  GS_NUM_STATES
} game_state;

enum {
  MAX_BINDINGS = 100
};

typedef enum binding_type {
  BINDING_ATOMIC,     // press button to turn state on,
                      // press again to switch off

  BINDING_CONTINUOUS, // press button and state turns on,
                      // release and state turns off

  BINDING_ONE_TIME    // press button and state turns on,
                      // state must then be turned off
                      // manually
} binding_type;

typedef struct key_state_binding {
  SDL_Keycode k;
  game_state s;
  binding_type t;
} key_state_binding;

typedef struct decal {
  SDL_Texture* texture;
  SDL_Rect rect;
} decal;

typedef struct sprite {
  int w;
  int h;
  int r[2];
  decal* d;
} sprite;
typedef struct sprite_list sprite_list;

typedef struct actor actor;
typedef void (*actor_render_handler)(actor* a);
typedef void (*actor_logic_handler)(actor* a);

struct actor {
  actor_render_handler render_handler;
  actor_logic_handler logic_handler;
};

typedef struct actor_list actor_list;
struct actor_list {
  actor* a;
  actor_list* next;
};

typedef struct apple_actor {
  actor a;
  sprite sprite;
} apple_actor;

typedef struct background_actor {
  actor a;
  sprite background_sprite;

  TTF_Font* font;
  SDL_Texture* status_bar;
  SDL_Texture* status_text;
} background_actor;

typedef struct snake_actor {
  actor a;
  int num_segments;
  unsigned int segment_w;
  unsigned int segment_h;

  sprite snake_sprites[MAX_SNAKE_SEGMENTS];
  DIRECTION direction;


  unsigned int snake_frame_ratio;
  unsigned int tail_index;

  bool needs_to_grow;
} snake_actor;

typedef struct context {
  unsigned int fps;
  unsigned int current_frame;
  SDL_Window* window;
  SDL_Renderer* renderer;
  unsigned int w;
  unsigned int h;

  Uint32 start_time;
  bool should_start_logic_loop;
  unsigned int whole_frames_to_do;

  actor_list* render_list;
  actor_list* logic_list;

  SDL_Texture* textures[NUM_TEXTURES];
  decal sprites_decals[NUM_SPRITES_DECALS];
  decal sand_decal;

  apple_actor apple_actor;
  background_actor background_actor;
  // status_bar_actor status_bar_actor;
  snake_actor snake_actor;

  unsigned int score;
  unsigned int* occupied_gridpoints;
  int grid_width;
  int grid_height;
} context;

extern context ctx;

#endif
