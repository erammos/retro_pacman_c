/**
 * @file
 * @author Elias Rammos <rammoselias@gmail.com>
 * @version $Date$
 */

#ifndef TILE_MAP_H
#define TILE_MAP_H

#include <stdint.h>
#ifdef tile_map_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

#include "common.h"

/* Constants declaration */

typedef struct
{
  uint8_t atlas_index;
  uint16_t rotation;

} tile;

/* Type declarations here */

typedef struct
{
  SDL_Texture *texture;
  char *grid;
  int foodCount;
  /* Specific fields of tile_map_Type here. */
} tile_map_t;

/* Global variables declarations */

/* Fuction prototypes here */

/**
 * Allocates a new tile_map_Type.
 * @return New tile_map_Type.
 */
EXTERN void tile_map_draw (SDL_Renderer *renderer);
EXTERN int tile_map_is_valid_move (direction dir, int x, int y);
EXTERN void tile_map_free (void);
EXTERN tile_map_t *tile_map_create (const char *, SDL_Texture * texture);
EXTERN char * tile_map_get_char (int x, int y);
#undef EXTERN
#endif
