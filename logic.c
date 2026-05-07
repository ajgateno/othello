#include <stdio.h>

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

void set_possible_move(game_t *game, int row, int column)
{
  if (game->board[row * N + column] != EMPTY) {
    return;
  }

  int opposite_player;
  if (game->player == BLACK) {
    opposite_player = WHITE;
  } else if (game->player == WHITE) {
    opposite_player = BLACK;
  }

  int i;

  // check left
  for (i = column - 1; i > 0; --i) {
    if (game->board[row * N + i] != opposite_player) {
      break;
    }
  }
  if (i < column - 1 && game->board[row * N + i] == game->player) {
    game->possible_moves[row * N + column].left = i;
  }

  // check right
  for (i = column + 1; i < N; ++i) {
    if (game->board[row * N + i] != opposite_player) {
      break;
    }
  }
  if (i > column + 1 && game->board[row * N + i] == game->player) {
    game->possible_moves[row * N + column].right = i;
  }

  // check up
  for (i = row - 1; i > 0; --i) {
    if (game->board[i * N + column] != opposite_player) {
      break;
    }
  }
  if (i < row - 1 && game->board[i * N + column] == game->player) {
    game->possible_moves[row * N + column].up = i;
  }

  // check down
  for (i = row + 1; i < N; ++i) {
    if (game->board[i * N + column] != opposite_player) {
      break;
    }
  }
  if (i > row + 1 && game->board[i * N + column] == game->player) {
    game->possible_moves[row * N + column].down = i;
  }
}

void set_possible_moves(game_t *game)
{
  // clear previous possible moves on the board
  for (int i = 0; i < N * N; ++i) {
    if (game->board[i] == POSSIBLE) {
      game->board[i] = EMPTY;
    }
  }

  // clear previous possible moves on possible moves buffer
  for (int i = 0; i < N * N; ++i) {
    game->possible_moves[i].up = -1;
    game->possible_moves[i].down = -1;
    game->possible_moves[i].left = -1;
    game->possible_moves[i].right = -1;
  }

  // Set all possible moves
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      set_possible_move(game, i, j);

      if (game->possible_moves[i * N + j].up > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].down > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].left > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].right > -1) {
        game->board[i * N + j] = POSSIBLE;
      }
    }
  }
}

void flip_cell(game_t *game, int row, int column)
{
  if (game->board[row * N + column] == BLACK) {
    game->board[row * N + column] = WHITE;
  } else if (game->board[row * N + column] == WHITE) {
    game->board[row * N + column] = BLACK;
  }
}

void flip_adjacent(game_t *game, int row, int column)
{
  // flip left
  if (game->possible_moves[row * N + column].left > -1) {
    for (int i = column - 1; i > game->possible_moves[row * N + column].left; --i) {
      flip_cell(game, row, i);
    }
  }

  // flip right
  if (game->possible_moves[row * N + column].right > -1) {
    for (int i = column + 1; i < game->possible_moves[row * N + column].right; ++i) {
      flip_cell(game, row, i);
    }
  }

  // flip up
  if (game->possible_moves[row * N + column].up > -1) {
    for (int i = row - 1; i > game->possible_moves[row * N + column].up; --i) {
      flip_cell(game, i, column);
    }
  }

  // flip down
  if (game->possible_moves[row * N + column].down > -1) {
    for (int i = row + 1; i < game->possible_moves[row * N + column].down; ++i) {
      flip_cell(game, i, column);
    }
  }
}

void player_turn(game_t *game, int row, int column)
{
  if (game->board[row * N + column] == POSSIBLE) {
    game->board[row * N + column] = game->player;
    flip_adjacent(game, row, column);
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
  game->board[3 * N + 3] = WHITE;
  game->board[3 * N + 4] = BLACK;
  game->board[4 * N + 3] = BLACK;
  game->board[4 * N + 4] = WHITE;
}

void click_on_cell(game_t *game, int row, int column)
{
  if (game->state == STATE_RUNNING) {
    player_turn(game, row, column);
  } else {
    reset_game(game);
  }
}
