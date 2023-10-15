#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL_render.h>

#define WINDOW_SCALE_X 2
#define WINDOW_SCALE_Y 2
#define TILE(x, y) y * 17 + x
#define MAX(a, b) (a > b) ? a : b
#define LEVEL_SIZE 33
#define TILE_SIZE 20
#define SCREEN_WIDTH TILE_SIZE *LEVEL_SIZE *WINDOW_SCALE_X
#define SCREEN_HEIGHT TILE_SIZE *LEVEL_SIZE *WINDOW_SCALE_Y
#define MAX_TABLE 256

#define PACMAN_LEFT TILE (4, 3)
#define PACMAN_DOWN TILE (4, 5)
#define PACMAN_RIGHT TILE (7, 4)
#define PACMAN_UP TILE (0, 6)

#define PINK_ENEMY_LEFT TILE (4, 1)
#define PINK_ENEMY_RIGHT TILE (5, 1)
#define PINK_ENEMY_BOTTOM TILE (6, 1)
#define PINK_ENEMY_TOP TILE (7, 1)

#define BLUE_ENEMY_LEFT TILE (0, 1)
#define BLUE_ENEMY_RIGHT TILE (1, 1)
#define BLUE_ENEMY_BOTTOM TILE (2, 1)
#define BLUE_ENEMY_TOP TILE (3, 1)

#define RED_ENEMY_LEFT TILE (8, 2)
#define RED_ENEMY_RIGHT TILE (9, 2)
#define RED_ENEMY_BOTTOM TILE (10, 2)
#define RED_ENEMY_TOP TILE (11, 2)

#define YELLOW_ENEMY_LEFT TILE (8, 1)
#define YELLOW_ENEMY_RIGHT TILE (9, 1)
#define YELLOW_ENEMY_BOTTOM TILE (10, 2)
#define YELLOW_ENEMY_TOP TILE (11, 2)

#define TILE_H_LINE '-'
#define TILE_V_LINE '|'
#define TILE_TL_CORNER '1'
#define TILE_TR_CORNER '2'
#define TILE_BR_CORNER '3'
#define TILE_BL_CORNER '4'
#define TILE_B_EDGE 'v'
#define TILE_T_EDGE '^'
#define TILE_L_EDGE '<'
#define TILE_R_EDGE '>'
#define TILE_B_CROSS 'V'
#define TILE_R_CROSS '.'
#define TILE_T_CROSS '6'
#define TILE_L_CROSS ','
#define TILE_FOOD '*'
#define TILE_SUPER_FOOD 'x'
#define START_POSITION 'c'
#define SPACE ' '

typedef enum
{
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  STOP
} DIRECTION;

typedef struct
{
  int x;
  int y;
} direction;

typedef struct
{
  SDL_Texture *texture;
  int x;
  int y;
  int atlas_index;
  direction dir;
} entity;

void blit (SDL_Renderer *renderer, SDL_Texture *texture, int index, int x,
           int y, float angle);
SDL_Texture *loadTexture (char *filename, SDL_Renderer *renderer);
#endif
