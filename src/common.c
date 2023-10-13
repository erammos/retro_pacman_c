#include "common.h"

void
blit (SDL_Renderer * renderer, SDL_Texture *texture, int index, int x, int y, float angle)
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
