
#define tile_map_IMPORT
#include "tile_map.h"
#include "common.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

tile_map_t tile_map = { 0 };

tile tile_table[MAX_TABLE]
    = { [TILE_H_LINE] = { .atlas_index = TILE (7, 13), .rotation = 0 },
        [TILE_V_LINE] = { .atlas_index = TILE (6, 13), .rotation = 0 },
        [TILE_TL_CORNER] = { .atlas_index = TILE (6, 10), .rotation = 0 },
        [TILE_B_CROSS] = { .atlas_index = TILE (7, 10), .rotation = 0 },
        [TILE_R_CROSS] = { .atlas_index = TILE (6, 11), .rotation = 0 },
        [TILE_L_CROSS] = { .atlas_index = TILE (8, 11), .rotation = 0 },
        [TILE_T_CROSS] = { .atlas_index = TILE (7, 12), .rotation = 0 },
        [TILE_TR_CORNER] = { .atlas_index = TILE (8, 10), .rotation = 0 },
        [TILE_BR_CORNER] = { .atlas_index = TILE (8, 12), .rotation = 0 },
        [TILE_BL_CORNER] = { .atlas_index = TILE (6, 12), .rotation = 0 },
        [TILE_B_EDGE] = { .atlas_index = TILE (9, 12), .rotation = 0 },
        [TILE_T_EDGE] = { .atlas_index = TILE (10, 12), .rotation = 0 },
        [TILE_R_EDGE] = { .atlas_index = TILE (10, 13), .rotation = 0 },
        [TILE_L_EDGE] = { .atlas_index = TILE (9, 13), .rotation = 0 },
        [TILE_FOOD] = { .atlas_index = TILE (12, 0), .rotation = 0 },
        [TILE_SUPER_FOOD] = { .atlas_index = TILE (13, 0), .rotation = 0 } };

tile_map_t *
tile_map_create (const char *filename, SDL_Texture *texture)
{
  FILE *file = fopen (filename, "r");
  char *line = NULL;
  ssize_t len = 0;
  size_t size = 0;
  tile_map.grid = malloc (LEVEL_SIZE * LEVEL_SIZE * sizeof (char));
  tile_map.texture = texture;

  int i = 0, j = 0;
  while ((len = getline (&line, &size, file)) != -1)
    {
      for (j = 0; j < len - 1; j++)
        {
          if (line[j] == TILE_FOOD || line[j] == TILE_SUPER_FOOD)
            {
              (tile_map.foodCount)++;
            }
          tile_map.grid[i * LEVEL_SIZE + j] = line[j];
        }
      i++;
    }

  fclose (file);
  return &tile_map;
}

char *
tile_map_get_char (int x, int y)
{
  int j = (x + TILE_SIZE / 2) / TILE_SIZE;
  int i = (y + TILE_SIZE / 2) / TILE_SIZE;
  return &(tile_map.grid[i * LEVEL_SIZE + j]);
}

void
tile_map_draw (SDL_Renderer *renderer)
{
  for (int i = 0; i < LEVEL_SIZE; i++)
    {
      for (int j = 0; j < LEVEL_SIZE; j++)
        {
          SDL_Rect rect = {
            .x = j * TILE_SIZE, .y = i * TILE_SIZE, TILE_SIZE, TILE_SIZE
          };

          SDL_RenderDrawRect (renderer, &rect);
          char c = tile_map.grid[i * LEVEL_SIZE + j];
          if (c != 0 && c != '\0' && c != '\n' && c != 'c' && c != ' ')
            {
              tile t = tile_table[c];
              blit (renderer, tile_map.texture, t.atlas_index, j * TILE_SIZE,
                    i * TILE_SIZE, t.rotation);
            }
        }
    }
}

int
tile_map_is_valid_move (direction dir, int x, int y)
{
  int curr_j = (x + TILE_SIZE / 2) / TILE_SIZE;
  int curr_i = (y + TILE_SIZE / 2) / TILE_SIZE;
  int next_j = curr_j + dir.x;
  int next_i = curr_i + dir.y;
  if (next_j < 0 || next_j >= LEVEL_SIZE)
    {
      return 1;
    }
  char tile = tile_map.grid[next_i * LEVEL_SIZE + next_j];
  return tile == SPACE || tile == TILE_FOOD || tile == TILE_SUPER_FOOD;
}

void
tile_map_free (void)
{
  free (tile_map.grid);
}
