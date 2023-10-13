#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "tile_map.h"


entity pacman = {
  .atlas_index = PACMAN_LEFT, .x = 100, .y = 100, .dir = { .x = 0, .y = 0 }
};

entity enemies[4] = {
  { .atlas_index = PACMAN_LEFT,
    .x = 100,
    .y = 100,
    .dir = { .x = 0, .y = 0 } },
  { .atlas_index = PACMAN_LEFT,
    .x = 100,
    .y = 100,
    .dir = { .x = 0, .y = 0 } },
  { .atlas_index = PACMAN_LEFT,
    .x = 100,
    .y = 100,
    .dir = { .x = 0, .y = 0 } },
  { .atlas_index = PACMAN_LEFT, .x = 100, .y = 100, .dir = { .x = 0, .y = 0 } }
};

int foodCount = 0;


SDL_Window *window = { 0 };
SDL_Renderer *renderer = { 0 };
SDL_Texture *texture = { 0 };

void
set_pacman_texture (entity *pacman)
{
  if (pacman->dir.x == 1)
    pacman->atlas_index = PACMAN_RIGHT;
  else if (pacman->dir.x == -1)
    pacman->atlas_index = PACMAN_LEFT;
  else if (pacman->dir.y == 1)
    pacman->atlas_index = PACMAN_DOWN;
  else
    pacman->atlas_index = PACMAN_UP;
}

void
enemy_update (int n, entity enemies[n], entity *pacman)
{

  for (int i = 0; i < n; i++)
    {
      direction dir;

      dir.x = enemies[i].x - pacman->x;
      dir.y = enemies[i].y - pacman->y;
      if (dir.x > 0)
        {
        }
      else
        {
        }
      if (dir.y > 0)
        {
        }
      else
        {
        }
    }
}
int
can_move (int x, int y)
{
  char * c = tile_map_get_char(x, y); 

  return *c == SPACE;
}

void
initialize_pacman_position (tile_map_t * tile_map)
{
  for (int i = 0; i < LEVEL_SIZE * LEVEL_SIZE; i++)
    {
      if (tile_map->grid[i] == 'c')
        {
          pacman.x = (i % LEVEL_SIZE) * TILE_SIZE;
          pacman.y = (i / LEVEL_SIZE) * TILE_SIZE;
          tile_map->grid[i] = ' ';
          return;
        }
    }
}


void
check_input (entity *pacman)
{
  SDL_Event event;
  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_QUIT:
          exit (0);
        case SDL_KEYDOWN:
          {
            direction dir = { 0, 0 };
            switch (event.key.keysym.scancode)
              {
              case SDL_SCANCODE_RIGHT:
                dir.x = 1;
                break;
              case SDL_SCANCODE_LEFT:
                dir.x = -1;
                break;
              case SDL_SCANCODE_DOWN:
                dir.y = 1;
                break;
              case SDL_SCANCODE_UP:
                dir.y = -1;
                break;
              }
            if (tile_map_is_valid_move (dir, pacman->x, pacman->y))
              {
                pacman->dir = dir;
              }
          }
          break;
        default:
          break;
        }
    }
}

void
pacman_eat_food (entity *pacman)
{

  char *c = tile_map_get_char (pacman->x, pacman->y);
  if (*c == TILE_FOOD || *c == TILE_SUPER_FOOD)
    {
      foodCount--;
      *c = SPACE;
    }
}

void
pacman_update (entity *pacman)
{
  if (tile_map_is_valid_move (pacman->dir, pacman->x, pacman->y))
    {
      if (pacman->x + pacman->dir.x < 0)
        {
          pacman->x = LEVEL_SIZE * TILE_SIZE - 1;
        }
      else if (pacman->x + pacman->dir.x >= LEVEL_SIZE * TILE_SIZE)
        {
          pacman->x = 0;
        }
      else
        pacman->x += pacman->dir.x * TILE_SIZE;

      pacman->y += pacman->dir.y * TILE_SIZE;
    }
  pacman_eat_food (pacman);
}

void
pacman_draw (entity *pacman)
{

  set_pacman_texture (pacman);
  blit (renderer, texture, pacman->atlas_index, pacman->x, pacman->y, 0);
}

SDL_Texture *
loadTexture (char *filename)
{
  SDL_Texture *texture;
  SDL_LogMessage (SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                  "Loading %s", filename);

  texture = IMG_LoadTexture (renderer, filename);
  return texture;
}

int
main (void)
{
  renderer = NULL;
  window = NULL;

  if (SDL_Init (SDL_INIT_VIDEO) < 0)
    {
      printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit (1);
    }
  window = SDL_CreateWindow ("Pacman", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, 0);
  if (!window)
    {
      printf ("Failed to open %d x %d window: %s\n", SCREEN_WIDTH,
              SCREEN_HEIGHT, SDL_GetError ());
    }

  SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer)
    {
      printf ("Couldn't create renderer: %s\n", SDL_GetError ());
      exit (1);
    }

texture = loadTexture ("./assets/pacman_textures.png");
 tile_map_t * tilemap =  tile_map_create ("./assets/map0",texture);
  initialize_pacman_position (tilemap);

  float delta_time = 0.0f;
  float move_delay = 0.0f;
  while (1)
    {

      float then = SDL_GetTicks ();
      check_input (&pacman);
      move_delay = MAX (0, move_delay - delta_time);
      if (move_delay == 0)
        {
          pacman_update (&pacman);
          move_delay = 5 * 20;
        }

      SDL_RenderSetScale (renderer, WINDOW_SCALE_X, WINDOW_SCALE_Y);
      // SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
      SDL_RenderClear (renderer);
      tile_map_draw (renderer);
      pacman_draw (&pacman);
      SDL_RenderPresent (renderer);
      SDL_Delay (1);
      delta_time = SDL_GetTicks () - then;
    }
}
