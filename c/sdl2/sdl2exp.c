#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include <SDL_ttf.h>

#include <gos/experimental/sdl2/sdl2.h>

#define GOS_SDL_INIT_DEFAULT -1
#define GOS_TTF_INIT_DEFAULT -1

#define GOS_SDL_POINT_COUNT 5

#define GOS_SDL2_FONT_FILE "C:\\Windows\\Fonts\\verdana.ttf"

static int sdlinit = GOS_SDL_INIT_DEFAULT;
static int ttfinit = GOS_TTF_INIT_DEFAULT;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Surface* surface = NULL;
static SDL_Texture* texture = NULL;

static TTF_Font* font = NULL;

static SDL_Color white = { 255, 255, 255 };
static SDL_Point points[GOS_SDL_POINT_COUNT];

static void shutdown();

int main(int argc, char** argv) {
  int textw, texth;
  SDL_Event sdlevent;
  SDL_Rect textrect;
  bool go;

  sdlinit = SDL_Init(SDL_INIT_VIDEO);
  if (sdlinit != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  ttfinit = TTF_Init();
  if (ttfinit != 0) {
    fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow(
    "Hello World!",     /* Title */
    100,                /* X */
    100,                /* Y */
    620,                /* W */
    387,                /* H */
    SDL_WINDOW_SHOWN);  /* Flags */
  if (window == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    shutdown();
    return EXIT_FAILURE;
  }

  surface = SDL_GetWindowSurface(window);

  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x12, 0x45, 0x7b));

  SDL_UpdateWindowSurface(surface);

  renderer = SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    shutdown();
    return EXIT_FAILURE;
  }

  font = TTF_OpenFont(GOS_SDL2_FONT_FILE, 24);
  if (font == NULL) {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
    shutdown();
    return EXIT_FAILURE;
  }

  SDL_Surface* textsurface = TTF_RenderText_Blended(font, "Hello World!", white);
  SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);

  textw = textsurface->w;
  texth = textsurface->h;

  textrect.x = 0;
  textrect.y = 0;
  textrect.w = textw;
  textrect.h = texth;

  SDL_RenderCopy(renderer, texttexture, NULL, &textrect);

  points[0].x = 0;
  points[0].y = 0;
  points[1].x = textw;
  points[1].y = 0;
  points[2].x = textw;
  points[2].y = texth;
  points[3].x = 0;
  points[3].y = texth;
  points[4].x = 0;
  points[4].y = 0;

  SDL_SetRenderDrawColor(renderer, 0x23, 0x34, 0x45, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLines(renderer, points, GOS_SDL_POINT_COUNT);

  SDL_RenderPresent(renderer);

  /* Wait two seconds */
  SDL_Delay(5000);

  SDL_DestroyTexture(texttexture);
  SDL_FreeSurface(textsurface);

  shutdown();
  return EXIT_SUCCESS;
}

void shutdown() {
  if (font != NULL) {
    TTF_CloseFont(font);
    font = NULL;
  }

  if (renderer != NULL) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }

  if (window != NULL) {
    SDL_DestroyWindow(window);
    window = NULL;
  }

  if (ttfinit == 0) {
    TTF_Quit();
    ttfinit = GOS_TTF_INIT_DEFAULT;
  }

  if (sdlinit == 0) {
    SDL_Quit();
    sdlinit = GOS_SDL_INIT_DEFAULT;
  }
}
