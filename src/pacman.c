#define pacman_IMPORT
#include "common.h"
#include "tile_map.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
pacman_eat_food (entity *pacman, tile_map_t *tile_map)
{

  char *c = tile_map_get_char (pacman->x, pacman->y);
  if (*c == TILE_FOOD || *c == TILE_SUPER_FOOD)
    {
      tile_map->foodCount--;
      *c = SPACE;
    }
}

void
pacman_update (entity *pacman, tile_map_t *tile_map)
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
  pacman_eat_food (pacman, tile_map);
}

void
pacman_draw (entity *pacman, SDL_Renderer *renderer)
{

  set_pacman_texture (pacman);
  blit (renderer, pacman->texture, pacman->atlas_index, pacman->x, pacman->y,
        0);
}

entity *
pacman_create (tile_map_t *tile_map, SDL_Texture *texture)
{
  entity *pacman = malloc (sizeof (entity));
  pacman->texture = texture;
  pacman->atlas_index = PACMAN_LEFT;
  pacman->dir.x = -1;
  pacman->dir.y = 0;
  for (int i = 0; i < LEVEL_SIZE * LEVEL_SIZE; i++)
    {
      if (tile_map->grid[i] == 'c')
        {
          pacman->x = (i % LEVEL_SIZE) * TILE_SIZE;
          pacman->y = (i / LEVEL_SIZE) * TILE_SIZE;
          tile_map->grid[i] = ' ';
          return pacman;
        }
    }
  return NULL;
}

int
pacman_check_input (entity *pacman)
{
  SDL_Event event;
  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
        case SDL_QUIT:
          return 0;
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
  return 1;
}
