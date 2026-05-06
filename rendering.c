#include <math.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "game.h"
#include "rendering.h"

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255 };
const SDL_Color BLACK_COLOR = { .r = 50, .g = 50, .b = 50 };
const SDL_Color WHITE_COLOR = { .r = 200, .g = 200, .b = 200 };
const SDL_Color POSSIBLE_COLOR = { .r = 255, .g = 100, .b = 100 };

void render_grid(SDL_Renderer *renderer, const SDL_Color *color)
{
  SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

  for (int i = 0; i < N; ++i) {
    SDL_RenderDrawLine(renderer, i * CELL_WIDTH, 0, i * CELL_WIDTH, SCREEN_HEIGHT);
    SDL_RenderDrawLine(renderer, 0, i * CELL_HEIGHT, SCREEN_WIDTH, i * CELL_HEIGHT);
  }
}

void render_piece(SDL_Renderer *renderer, int row, int column, const SDL_Color *color) 
{
  const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
  const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
  const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

  filledCircleRGBA(renderer, center_x, center_y, half_box_side + 5, color->r, color->g, color->b, 255);
}

void render_board(SDL_Renderer *renderer, const int *board, const SDL_Color *black_color, const SDL_Color *white_color)
{
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      switch (board[i * N + j]) {
        case BLACK:
          render_piece(renderer, i, j, black_color);
          break;

        case WHITE:
          render_piece(renderer, i, j, white_color);
          break;

        case POSSIBLE:
          render_piece(renderer, i, j, &POSSIBLE_COLOR);
          break;

        default: {}
      }
    }
  }
}

void render_running_state(SDL_Renderer *renderer, const game_t *game)
{
  render_grid(renderer, &GRID_COLOR);
  render_board(renderer, game->board, &BLACK_COLOR, &WHITE_COLOR);
}

void render_game_over_state(SDL_Renderer *renderer, const game_t *game, const SDL_Color *color)
{
  render_grid(renderer, color);
  render_board(renderer, game->board, color, color);
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
