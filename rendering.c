#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "game.h"
#include "rendering.h"

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255 };
const SDL_Color BLACK_COLOR = { .r = 50, .g = 50, .b = 50 };
const SDL_Color WHITE_COLOR = { .r = 200, .g = 200, .b = 200 };

void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

  for (int i = 0; i < N; ++i) {
    SDL_RenderDrawLine(renderer, i * CELL_WIDTH, 0, i * CELL_WIDTH, SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, 0, i * CELL_HEIGHT, SCREEN_WIDTH, i * CELL_HEIGHT);
  }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
  render_grid(renderer, &GRID_COLOR);
  // TODO render board
}

void render_game_over_state(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color)
{
  render_grid(renderer, color);
  // TODO render board
}

void render_game(SDL_Renderer *renderer, const game_t *game)
{
  switch (game->state) {
    case STATE_RUNNING:
      render_running_state(renderer, game);
      break;

    case STATE_BLACK_WON:
      render_game_over_state(renderer, game, &BLACK_COLOR);
      break;

    case STATE_WHITE_WON:
      render_game_over_state(renderer, game, &WHITE_COLOR);
      break;

    case STATE_DRAW:
      render_running_state(renderer, game);
      break;

    default: {}
  }
}
