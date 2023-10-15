#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <stdint.h>
#ifdef tile_map_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

#include "common.h"

typedef struct
{
  uint8_t atlas_index;
  uint16_t rotation;

} tile;

typedef struct
{
  SDL_Texture *texture;
  char *grid;
  int foodCount;
} tile_map_t;

EXTERN void tile_map_draw (SDL_Renderer *renderer);
EXTERN int tile_map_is_valid_move (direction dir, int x, int y);
EXTERN void tile_map_free (void);
EXTERN tile_map_t *tile_map_create (const char *, SDL_Texture *texture);
EXTERN char *tile_map_get_char (int x, int y);
#undef EXTERN
#endif
