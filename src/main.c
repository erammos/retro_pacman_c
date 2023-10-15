#include "common.h"
#include "pacman.h"
#include "tile_map.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>

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
  tile_map_t *tilemap = tile_map_create ("./assets/map0", texture);

  entity *pacman = pacman_create (tilemap, texture);

  float delta_time = 0.0f;
  float move_delay = 0.0f;
  while (1)
    {

      float then = SDL_GetTicks ();
      pacman_check_input (pacman);
      move_delay = MAX (0, move_delay - delta_time);
      if (move_delay == 0)
        {
          pacman_update (pacman, tilemap);
          move_delay = 5 * 20;
        }

      SDL_RenderSetScale (renderer, WINDOW_SCALE_X, WINDOW_SCALE_Y);
      SDL_RenderClear (renderer);
      tile_map_draw (renderer);
      pacman_draw (pacman, renderer);
      SDL_RenderPresent (renderer);
      SDL_Delay (1);
      delta_time = SDL_GetTicks () - then;
    }
}
