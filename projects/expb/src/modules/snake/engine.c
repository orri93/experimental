#include <string.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL_image.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <modules/snake/types.h>
#include <modules/snake/macros.h>
#include <modules/snake/engine.h>
#include <modules/snake/input_processor.h>
#include <modules/snake/background_actor.h>
#include <modules/snake/apple_actor.h>
#include <modules/snake/snake_actor.h>
#include <modules/snake/decal.h>
#include <modules/snake/actor.h>

context ctx;

static void setup_textures(const char* imgdir);
static void setup_decals();
static void setup_actors(const char* fontdir);
static void loop_handler();
static bool should_continue_logic_loops();
static void setup_bindings();
static bool loop();

bool engine_init(
  const char* imgdir,
  const char* fontdir,
  unsigned int w,
  unsigned int h) {
  ctx.fps = 80;
  ctx.current_frame = 0;
  ctx.score = 0;

  ctx.start_time = 0;
  ctx.should_start_logic_loop = true;
  ctx.whole_frames_to_do = 0;

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_CreateWindowAndRenderer(w, h, 0, &ctx.window, &ctx.renderer);
  ctx.w = w;
  ctx.h = h;

  if(ctx.window == NULL) {
    fprintf(
      stderr,
      "Window could not be created: %s\n", SDL_GetError());
    return false;
  }

  setup_textures(imgdir);
  setup_decals();

  ctx.grid_width = w / TILE_DIMENSION;
  ctx.grid_height = (h / TILE_DIMENSION) - STATUS_BAR_HEIGHT;
  ctx.occupied_gridpoints = malloc(ctx.grid_width * ctx.grid_height * sizeof(*ctx.occupied_gridpoints));
  memset(ctx.occupied_gridpoints, 0, ctx.grid_width * ctx.grid_height * sizeof(*ctx.occupied_gridpoints));

  SDL_StartTextInput();
  setup_bindings();

  ctx.render_list = NULL;
  ctx.logic_list = NULL;

  setup_actors(fontdir);

  return true;
}

void engine_destroy() {
  SDL_Quit();
}

bool engine_start() {
  ctx.start_time = SDL_GetTicks();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop_handler, -1, 0);
#else
  if (!loop()) {
    return false;
  }
#endif

  return true;
}

void engine_reset() {
  snake_actor_init(&ctx.snake_actor);
  memset(ctx.occupied_gridpoints, 0, ctx.grid_width * ctx.grid_height * sizeof(*ctx.occupied_gridpoints));
  apple_actor_init(&ctx.apple_actor);
  ctx.score = 0;
  background_actor_update_scoreboard(&ctx.background_actor);
}

void setup_textures(const char* imgdir) {
  const char* psep = SNAKE_PATH_SEP;
  char fname[FILENAME_MAX];
  size_t lpos;

  char * filenames[] = {
    "sand.png",
    "sprites.png",
    "status_bar.png"
  };
  int i;

  for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
    if (imgdir != NULL) {
      lpos = strlen(imgdir);
      strncpy(fname, imgdir, lpos + 1);
      if (lpos > 0 && imgdir[lpos - 1] != psep[0]) {
        strcat(fname, psep);
      }
      strcat(fname, filenames[i]);
    } else {
      strcpy(fname, filenames[i]);
    }
    SDL_Surface *img = IMG_Load(fname);

    if(!img){
      fprintf(stdout, "Error! Could not load %s\n", fname);
      exit(1);
    }
        
    ctx.textures[i] = SDL_CreateTextureFromSurface(ctx.renderer, img);

    SDL_FreeSurface(img);
  }
}

void setup_decals() {
  int i;

  int imgw, imgh;
  SDL_QueryTexture(ctx.textures[SPRITES_TEXTURE],
          NULL, NULL, &imgw, &imgh);

  for (i = 0; i < NUM_SPRITES_DECALS; i++) {
    double w = 1/8. * imgw;
    double h = 1/2. * imgh;
    double x = (double)(i % 8) * w;
    double y = (double)(i / 8) * h;

    decal_init(
      &ctx.sprites_decals[i],
      ctx.textures[SPRITES_TEXTURE],
      x,
      y,
      w,
      h);
  }

  SDL_QueryTexture(ctx.textures[SAND_TEXTURE],
    NULL, NULL, &imgw, &imgh);

  decal_init(
    &(ctx.sand_decal),
    ctx.textures[SAND_TEXTURE],
    0,
    0,
    imgw,
    imgh);
}

void setup_actors(const char* fontdir) {
  snake_actor_init(&ctx.snake_actor);
  ctx.render_list = actor_list_add(ctx.render_list, (actor *)(&ctx.snake_actor));
  ctx.logic_list = actor_list_add(ctx.logic_list, (actor *)(&ctx.snake_actor));

  apple_actor_init(&ctx.apple_actor);
  ctx.render_list = actor_list_add(ctx.render_list, (actor *)(&ctx.apple_actor));

  background_actor_init(fontdir, &ctx.background_actor);
  ctx.render_list = actor_list_add(ctx.render_list, (actor *)(&ctx.background_actor));
}

bool should_continue_logic_loops() {
  if (ctx.should_start_logic_loop) {
    unsigned int logic_loop_start_time = SDL_GetTicks();
    double elapsed_frames = (double)(logic_loop_start_time \
      - ctx.start_time) / 1000.0f * ctx.fps;

    ctx.whole_frames_to_do = (unsigned int)elapsed_frames - ctx.current_frame;
  }

  if (!ctx.whole_frames_to_do) {
    ctx.should_start_logic_loop = true;
    return false;
  }

  ctx.whole_frames_to_do -= 1;
  ctx.current_frame += 1;
  ctx.should_start_logic_loop = false;
  return true;
}

bool handler(bool* goer) {
  process_input(goer);

  SDL_RenderClear(ctx.renderer);

  actor_list* al;
  for (al = ctx.render_list; al != NULL; al = al->next) {
    al->a->render_handler(al->a);
  }

  while (should_continue_logic_loops()) {
    for (al = ctx.logic_list; al != NULL; al = al->next) {
      al->a->logic_handler(al->a);
    }
  }

  SDL_RenderPresent(ctx.renderer);

  return true;
}

void loop_handler() {
  handler(NULL);
}

bool loop() {
  bool go = true;
  while (go) {
    if (!handler(&go)) {
      return false;
    }
  }
  return true;
}

void setup_bindings() {
  input_processor_init();
  key_state_binding binding;

  binding.k = SDLK_UP;
  binding.s = GS_N;
  binding.t = BINDING_ONE_TIME;
  add_binding(&binding);

  binding.k = SDLK_RIGHT;
  binding.s = GS_E;
  add_binding(&binding);

  binding.k = SDLK_DOWN;
  binding.s = GS_S;
  add_binding(&binding);

  binding.k = SDLK_LEFT;
  binding.s = GS_W;
  add_binding(&binding);
}

void engine_get_grid_coord(const int *pixel_coord, int *grid_coord) {
  grid_coord[0] = pixel_coord[0] / TILE_DIMENSION;
  grid_coord[1] = pixel_coord[1] / TILE_DIMENSION - STATUS_BAR_HEIGHT;
}

void engine_get_pixel_coord(const int *grid_coord, int *pixel_coord) {
  pixel_coord[0] = grid_coord[0] * TILE_DIMENSION;
  pixel_coord[1] = (grid_coord[1] + STATUS_BAR_HEIGHT) * TILE_DIMENSION;
}

void engine_apply_boundary_conditions(int *grid_coords) {
  grid_coords[0] = ((grid_coords[0] % ctx.grid_width) + ctx.grid_width) % ctx.grid_width;
  grid_coords[1] = ((grid_coords[1] % ctx.grid_height) + ctx.grid_height) % ctx.grid_height;
}
