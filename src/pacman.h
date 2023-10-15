#ifndef PACMAN_H
#define PACMAN_H

#include "common.h"
#include "tile_map.h"
#include <stdint.h>
#ifdef pacman_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN void set_pacman_texture (entity *pacman);
EXTERN void pacman_eat_food (entity *pacman, tile_map_t *tile_map);
EXTERN void pacman_update (entity *pacman, tile_map_t *tile_map);
EXTERN void pacman_draw (entity *pacman, SDL_Renderer *renderer);
EXTERN entity *pacman_create (tile_map_t *tile_map, SDL_Texture *texture);
EXTERN int pacman_check_input (entity *pacman);
#undef EXTERN
#endif
