#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

#include <modules/owl.h>
#endif

#define OWL_SDL_INIT_DEFAULT -1
#define OWL_TTF_INIT_DEFAULT -1
#define OWL_IMG_INIT_DEFAULT 0

#define OWL_ASSETS_IMG_OWL_PNG "assets/img/owl.png"
#define OWL_ASSETS_FONT_FREE_SANS_TTF "assets/font/FreeSans.ttf"

/**
 * Inverse square root of two, for normalising velocity
 */
#define REC_SQRT2 0.7071067811865475 

 /**
  * Set of input states
  */
enum input_state {
  NOTHING_PRESSED = 0,
  UP_PRESSED = 1,
  DOWN_PRESSED = 1 << 1,
  LEFT_PRESSED = 1 << 2,
  RIGHT_PRESSED = 1 << 3
};

/**
 * Context structure that will be passed to the loop handler
 */
struct context {
  int sdlinit;
  int ttfinit;
  int imginit;

  int speed;

  SDL_Renderer* renderer;

  /**
   * Rectangle that the owl texture will be rendered into
   */
  SDL_Rect dest;
  SDL_Texture* owl_tex;

  /**
   * Font that is used for rendering text, and
   * a texture the text is rendered into
   */
  TTF_Font* font;
  SDL_Texture* text_tex;

  enum input_state active_state;

  /**
   * x and y components of owl's velocity
   */
  int owl_vx;
  int owl_vy;
};

static SDL_Window* window;
static struct context ctx;

/**
 * Loads the owl texture into the context
 */
int get_owl_texture(struct context* ctx, const char* ifn) {
  SDL_Surface* owlimg;
  owlimg = IMG_Load(ifn);
  if (owlimg == NULL) {
    fprintf(stderr, "Failed Load the Image file %s: %s\n", ifn, IMG_GetError());
    return 0;
  }
  ctx->owl_tex = SDL_CreateTextureFromSurface(ctx->renderer, owlimg);
  ctx->dest.w = owlimg->w;
  ctx->dest.h = owlimg->h;

  SDL_FreeSurface(owlimg);

  return 1;
}


/**
 * Set the context's text texture to show the text 'text'
 */
void set_font_text(struct context* ctx, const char* text) {
  SDL_Color fg = { 0,0,0,255 };
  SDL_Surface* text_surface = TTF_RenderText_Blended(ctx->font, text, fg);
  ctx->text_tex = SDL_CreateTextureFromSurface(ctx->renderer, text_surface);
  SDL_FreeSurface(text_surface);
}

/**
 * Load the font we're going to use and set the text to
 * be "Hello owl!"
 */
int get_font_texture(struct context* ctx, const char* ffn) {
  ctx->font = TTF_OpenFont(ffn, 30);
  if (ctx->font == NULL) {
    fprintf(stderr, "Failed Load the Font file %s: %s\n", ffn, TTF_GetError());
    return 0;
  }
  set_font_text(ctx, "Hello owl!");
  return 1;
}

/**
 * Processes the input events and sets the velocity
 * of the owl accordingly
 */
void process_input(struct context* ctx, bool* goer) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      if (goer != NULL) {
        *goer = false;
      }
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_q:
        if (goer != NULL) {
          *goer = false;
        }
        break;
      case SDLK_m:
        ctx->dest.x = 200;
        ctx->dest.y = 100;
        break;
      case SDLK_UP:
        if (event.key.type == SDL_KEYDOWN)
          ctx->active_state |= UP_PRESSED;
        else if (event.key.type == SDL_KEYUP)
          ctx->active_state ^= UP_PRESSED;
        break;
      case SDLK_DOWN:
        if (event.key.type == SDL_KEYDOWN)
          ctx->active_state |= DOWN_PRESSED;
        else if (event.key.type == SDL_KEYUP)
          ctx->active_state ^= DOWN_PRESSED;
        break;
      case SDLK_LEFT:
        if (event.key.type == SDL_KEYDOWN)
          ctx->active_state |= LEFT_PRESSED;
        else if (event.key.type == SDL_KEYUP)
          ctx->active_state ^= LEFT_PRESSED;
        break;
      case SDLK_RIGHT:
        if (event.key.type == SDL_KEYDOWN)
          ctx->active_state |= RIGHT_PRESSED;
        else if (event.key.type == SDL_KEYUP)
          ctx->active_state ^= RIGHT_PRESSED;
        break;
      default:
        /* Do nothing */
        break;
      }
      break;
    default:
      /* Do nothing */
      break;
    }
  }

  ctx->owl_vy = 0;
  ctx->owl_vx = 0;
  if (ctx->active_state & UP_PRESSED)
    ctx->owl_vy = -ctx->speed;
  if (ctx->active_state & DOWN_PRESSED)
    ctx->owl_vy = ctx->speed;
  if (ctx->active_state & LEFT_PRESSED)
    ctx->owl_vx = -ctx->speed;
  if (ctx->active_state & RIGHT_PRESSED)
    ctx->owl_vx = ctx->speed;

  if (ctx->owl_vx != 0 && ctx->owl_vy != 0) {
    ctx->owl_vx *= REC_SQRT2;
    ctx->owl_vy *= REC_SQRT2;
  }
}

void loop(struct context* ctx, bool* goer) {
  int vx = 0;
  int vy = 0;
  process_input(ctx, goer);

  ctx->dest.x += ctx->owl_vx;
  ctx->dest.y += ctx->owl_vy;

  SDL_SetRenderDrawColor(ctx->renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(ctx->renderer);
  SDL_RenderCopy(ctx->renderer, ctx->owl_tex, NULL, &ctx->dest);

  SDL_Rect text_dest = { .x = 50, .y = 175, .w = 0, .h = 0 };
  SDL_QueryTexture(ctx->text_tex,
    NULL, NULL, &text_dest.w, &text_dest.h);
  SDL_RenderCopy(ctx->renderer, ctx->text_tex, NULL, &text_dest);

  SDL_RenderPresent(ctx->renderer);
}

/**
 * Loop handler that gets called each animation frame,
 * process the input, update the position of the owl and
 * then render the texture
 */
void loop_handler(void* arg) {
  struct context* ctx = arg;
  loop(ctx, false);
}

bool initialize(const char* owlpngfile, const char* fontfile) {
  int createwinrend;

  window = NULL;
  ctx.speed = 5;
  ctx.renderer = NULL;
  ctx.dest.x = 0;
  ctx.dest.y = 0;
  ctx.dest.w = 0;
  ctx.dest.h = 0;
  ctx.owl_tex = NULL;
  ctx.font = NULL;
  ctx.text_tex = NULL;
  ctx.active_state = NOTHING_PRESSED;
  ctx.owl_vx = 0;
  ctx.owl_vy = 0;

  ctx.sdlinit = OWL_SDL_INIT_DEFAULT;
  ctx.ttfinit = OWL_TTF_INIT_DEFAULT;
  ctx.imginit = OWL_IMG_INIT_DEFAULT;

  ctx.sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (ctx.sdlinit != 0) {
    fprintf(stderr, "SDL Initialization failed: %s\n", SDL_GetError());
    return false;
  }

  ctx.ttfinit = TTF_Init();
  if (ctx.ttfinit != 0) {
    fprintf(stderr, "SDL TTF Initialization failed: %s\n", TTF_GetError());
    return false;
  }

  ctx.imginit = IMG_Init(IMG_INIT_PNG);
  if ((ctx.imginit & IMG_INIT_PNG) == 0) {
    fprintf(stderr, "SDL Image Initialization failed: %s\n", IMG_GetError());
    return false;
  }

  createwinrend = SDL_CreateWindowAndRenderer(
    600,
    400,
    0,
    &window,
    &(ctx.renderer));
  if (createwinrend != 0) {
    fprintf(
      stderr, "SDL Create Window and Renderer failed: %s\n", SDL_GetError());
    return false;
  }

  if (get_owl_texture(&ctx, owlpngfile) == 0) {
    return false;
  }
  if (get_font_texture(&ctx, fontfile) == 0) {
    return false;
  }

  ctx.active_state = NOTHING_PRESSED;
  ctx.dest.x = 200;
  ctx.dest.y = 100;
  ctx.owl_vx = 0;
  ctx.owl_vy = 0;

  return true;
}

void owlshutdown() {
  if (ctx.owl_tex != NULL) {
    SDL_DestroyTexture(ctx.owl_tex);
    ctx.owl_tex = NULL;
  }

  if (ctx.text_tex != NULL) {
    SDL_DestroyTexture(ctx.text_tex);
    ctx.text_tex = NULL;
  }

  if (ctx.renderer != NULL) {
    SDL_DestroyRenderer(ctx.renderer);
    ctx.renderer = NULL;
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  if (ctx.imginit != 0) {
    IMG_Quit();
    ctx.imginit = OWL_IMG_INIT_DEFAULT;
  }

  if (ctx.ttfinit == 0) {
    TTF_Quit();
    ctx.ttfinit = OWL_TTF_INIT_DEFAULT;
  }

  if (ctx.sdlinit == 0) {
    SDL_Quit();
    ctx.sdlinit = OWL_SDL_INIT_DEFAULT;
  }
}

#ifdef __EMSCRIPTEN__
int owl() {
  const char* owlfile = OWL_WASM_ASSETS_IMG_OWL_PNG;
  const char* fontfile = OWL_WASM_ASSETS_FONT;

  if (!initialize(owlfile, fontfile)) {
    return EXIT_FAILURE;
  }

  /**
   * Schedule the main loop handler to get
   * called on each animation frame
   */
  emscripten_set_main_loop_arg(loop_handler, &ctx, -1, 1);

  return EXIT_SUCCESS;
}

EMSCRIPTEN_KEEPALIVE void shutdown() {
  printf("Shutting down the Owl!\n");
  owlshutdown();
}

int main(int argc, char** argv) {
  printf("Initialize the Owl!\n");
  owl();
}
#else
int main(int argc, char** argv) {
  bool go;
  const char* owlfile = OWL_ASSETS_IMG_OWL_PNG;
  const char* fontfile = OWL_ASSETS_FONT_FREE_SANS_TTF;

  if(argc > 1) {
    owlfile = argv[1];
  }
  if (argc > 2) {
    fontfile = argv[2];
  }

  if (!initialize(owlfile, fontfile)) {
    owlshutdown();
    return EXIT_FAILURE;
  }

  ctx.speed = 1;

  go = true;
  while (go) {
    loop(&ctx, &go);
  }

  owlshutdown();

  return EXIT_SUCCESS;
}
#endif
