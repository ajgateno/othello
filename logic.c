#include "game.h"
#include "logic.h"

void switch_player(game_t *game)
{
  if (game->player == BLACK) {
    game->player = WHITE;
  } else if (game->player == WHITE) {
    game->player = BLACK;
  }
}

void player_turn(game_t *game, int row, int column)
{
  if (game->board[row * N + column] == EMPTY) {
    game->board[row * N + column] = game->player;
    switch_player(game);
    // TODO: check if the game is over
  }
}

void reset_game(game_t *game)
{
  game->player = BLACK;
  game->state = STATE_RUNNING;
  for (int i = 0; i < N * N; ++i) {
    game->board[i] = EMPTY;
  }
}

void click_on_cell(game_t *game, int row, int column)
{
  if (game->state == STATE_RUNNING) {
    player_turn(game, row, column);
  } else {
    reset_game(game);
  }
}
