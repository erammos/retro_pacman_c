#include "common.h"
#include "pacman.h"
#include "tile_map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct
{
  entity *pacman;
  tile_map_t *tilemap;
  SDL_Renderer *renderer;
  float move_delay;
  float delta_time;
  int is_running;
} game_state;

static void
mainLoop (game_state *state)
{
  if (!state->is_running)
    {
#ifdef __EMSCRIPTEN__
      emscripten_cancel_main_loop ();
#else
      exit (0);
#endif
    }

  float then = SDL_GetTicks ();
  state->is_running = pacman_check_input (state->pacman);
  state->move_delay = MAX (0, state->move_delay - state->delta_time);
  if (state->move_delay == 0)
    {
      pacman_update (state->pacman, state->tilemap);
      state->move_delay = 5 * 20;
    }

  SDL_RenderSetScale (state->renderer, WINDOW_SCALE_X, WINDOW_SCALE_Y);
  SDL_RenderClear (state->renderer);
  tile_map_draw (state->renderer);
  pacman_draw (state->pacman, state->renderer);
  SDL_RenderPresent (state->renderer);
  SDL_Delay (1);
  state->delta_time = SDL_GetTicks () - then;
}

int
main (void)
{

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }

  SDL_Window *window = SDL_CreateWindow ("Pacman", SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                         SCREEN_HEIGHT, 0);
  if (!window)
    {
      printf ("Failed to open %d x %d window: %s\n", SCREEN_WIDTH,
              SCREEN_HEIGHT, SDL_GetError ());
    }

  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL_Renderer *renderer
      = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    {
      printf ("Couldn't create renderer: %s\n", SDL_GetError ());
      exit (1);
    }

  SDL_Texture *texture
      = loadTexture ("./assets/pacman_textures.png", renderer);
  if (texture == NULL)
    {
      printf ("Couldn't load texture\n");
      exit (1);
    }
  tile_map_t *tilemap = tile_map_create ("./assets/map0", texture);
  entity *pacman = pacman_create (tilemap, texture);

  game_state state = { .delta_time = 0.0f,
                       .move_delay = 0.0f,
                       .tilemap = tilemap,
                       .pacman = pacman,
                       .renderer = renderer,
                       .is_running = 1 };
#ifdef __EMSCRIPTEN__
  printf ("Set main loop\n");
  emscripten_set_main_loop_arg ((em_arg_callback_func)mainLoop, (void *)&state,
                                0, 1);
#else
  while (1)
    {
      mainLoop (&state);
    }
#endif
}
