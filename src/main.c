#include <SDL2/SDL.h>
#include <SDL2/SDL_haptic.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>

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

typedef struct
{
  uint8_t atlas_index;
  uint16_t rotation;

} tile;

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
  int x;
  int y;
  int atlas_index;
  direction dir;
} entity;

entity pacman = {
  .atlas_index = PACMAN_LEFT, .x = 100, .y = 100, .dir = { .x = 0, .y = 0 }
};

int foodCount = 0;

char tile_map[LEVEL_SIZE * LEVEL_SIZE] = { 0 };

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
tilemap_create (const char *filename)
{
  FILE *file = fopen (filename, "r");
  char *line = NULL;
  ssize_t len = 0;
  size_t size = 0;

  int i = 0, j = 0;
  while ((len = getline (&line, &size, file)) != -1)
    {
      for (j = 0; j < len - 1; j++)
        {
          if (line[j] == TILE_FOOD || line[j] == TILE_SUPER_FOOD)
            {
              foodCount++;
            }
          tile_map[i * LEVEL_SIZE + j] = line[j];
        }
      i++;
    }
  fclose (file);
}

char *
tilemap_get_char (int x, int y)
{
  int j = (x + TILE_SIZE / 2) / TILE_SIZE;
  int i = (y + TILE_SIZE / 2) / TILE_SIZE;
  return &tile_map[i * LEVEL_SIZE + j];
}

int
can_move (int x, int y)
{
  return *tilemap_get_char (x, y) == SPACE;
}

void
initialize_pacman_position ()
{
  for (int i = 0; i < LEVEL_SIZE * LEVEL_SIZE; i++)
    {
      if (tile_map[i] == 'c')
        {
          pacman.x = (i % LEVEL_SIZE) * TILE_SIZE;
          pacman.y = (i / LEVEL_SIZE) * TILE_SIZE;
          tile_map[i] = ' ';
          return;
        }
    }
}

int
is_valid_move (direction dir, int x, int y)
{
  int curr_j = (x + TILE_SIZE / 2) / TILE_SIZE;
  int curr_i = (y + TILE_SIZE / 2) / TILE_SIZE;
  int next_j = curr_j + dir.x;
  int next_i = curr_i + dir.y;
  if (next_j < 0 || next_j >= LEVEL_SIZE)
    {
      return 1;
    }
  char tile = tile_map[next_i * LEVEL_SIZE + next_j];
  return tile == SPACE || tile == TILE_FOOD || tile == TILE_SUPER_FOOD;
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
            if (is_valid_move (dir, pacman->x, pacman->y))
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
pacman_move (entity *pacman)
{
  if (is_valid_move (pacman->dir, pacman->x, pacman->y))
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
}

void
pacman_eat_food (entity *pacman)
{

  char *c = tilemap_get_char (pacman->x, pacman->y);
  if (*c == TILE_FOOD || *c == TILE_SUPER_FOOD)
    {
      foodCount--;
      *c = SPACE;
    }
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

void
blit (SDL_Texture *texture, int index, int x, int y, float angle)
{
  int w, h;
  SDL_QueryTexture (texture, NULL, NULL, &w, &h);

  int tileWidth = TILE_SIZE + 1;
  int tileHeight = TILE_SIZE + 1;

  int numColumns = w / tileWidth;

  int row = index / numColumns;
  int col = index % numColumns;
  SDL_Rect src = { .x = col * tileWidth + 1,
                   .y = row * tileHeight + 1,
                   .w = tileWidth - 1,
                   .h = tileHeight - 1 };
  SDL_Rect dest = { .x = x, .y = y, .w = tileWidth, .h = tileHeight };
  SDL_RenderCopyEx (renderer, texture, &src, &dest, angle, NULL,
                    SDL_FLIP_NONE);
}

void
tilemap_draw ()
{
  for (int i = 0; i < LEVEL_SIZE; i++)
    {
      for (int j = 0; j < LEVEL_SIZE; j++)
        {
          SDL_Rect rect = {
            .x = j * TILE_SIZE, .y = i * TILE_SIZE, TILE_SIZE, TILE_SIZE
          };

          // SDL_SetRenderDrawColor (renderer, 255, 0, 0, 255);
          SDL_RenderDrawRect (renderer, &rect);
          char c = tile_map[i * LEVEL_SIZE + j];
          if (c != 0 && c != '\0' && c != '\n' && c != 'c' && c != ' ')
            {
              tile t = tile_table[c];
              blit (texture, t.atlas_index, j * TILE_SIZE, i * TILE_SIZE,
                    t.rotation);
            }
        }
    }
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

  tilemap_create ("./assets/map0");
  initialize_pacman_position ();
  texture = loadTexture ("./assets/pacman_textures.png");

  float delta_time = 0.0f;
  float move_delay = 0.0f;
  while (1)
    {

      float then = SDL_GetTicks ();
      check_input (&pacman);
      move_delay = MAX (0, move_delay - delta_time);
      if (move_delay == 0)
        {
          pacman_move (&pacman);
          pacman_eat_food (&pacman);
          move_delay = 5 * 20;
        }

      SDL_RenderSetScale (renderer, WINDOW_SCALE_X, WINDOW_SCALE_Y);
      // SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
      SDL_RenderClear (renderer);
      tilemap_draw ();
      set_pacman_texture (&pacman);
      blit (texture, pacman.atlas_index, pacman.x, pacman.y, 0);
      SDL_RenderPresent (renderer);
      SDL_Delay (1);
      delta_time = SDL_GetTicks () - then;
    }
}
