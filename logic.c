#include <stdio.h>

#include "game.h"
#include "logic.h"

int row_chains[2][N * N];
int col_chains[2][N * N];

void switch_player(game_t *game)
{
  if (game->player == BLACK) {
    game->player = WHITE;
  } else if (game->player == WHITE) {
    game->player = BLACK;
  }
}

void set_possible_moves(game_t *game)
{
  // clear all possible moves from the previous turn
  for (int i = 0; i < N * N; ++i) {
    if (game->board[i] == POSSIBLE) {
      game->board[i] = EMPTY;
    }
  }

  // clear all previously set endpoints
  for (int i = 0; i < N * N; ++i) {
    game->row_endpoints[0 * (N * N) + i] = -1;
    game->row_endpoints[1 * (N * N) + i] = -1;

    game->col_endpoints[0 * (N * N) + i] = -1;
    game->col_endpoints[1 * (N * N) + i] = -1;
  }

  int opposite_player;
  if (game->player == BLACK) {
    opposite_player = WHITE;
  } else if (game->player == WHITE) {
    opposite_player = BLACK;
  }

  // for each row and column, figure out all intervals of consecutive opposite pieces
  int num_row_chains = 0;
  int in_row_chain = 0;

  int num_col_chains = 0;
  int in_col_chain = 0;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      // Entering a row chain
      if (!in_row_chain && game->board[i * N + j] == opposite_player) {
        in_row_chain = 1;
        row_chains[num_row_chains][0] = i * N + j;
      }

      // Leaving a row chain
      if (in_row_chain) {
        // Broken because the current block is not in the chain
        if (game->board[i * N + j] != opposite_player) {
          in_row_chain = 0;
          row_chains[num_row_chains][1] = i * N + j - 1;
          ++num_row_chains;
        }

        // Broken because we're at the end of the row
        if (j == N - 1) {
          in_row_chain = 0;
          row_chains[num_row_chains][1] = i * N + j;
          ++num_row_chains;
        }
      }

      // Entering a col chain
      if (!in_col_chain && game->board[j * N + i] == opposite_player) {
        in_col_chain = 1;
        col_chains[num_col_chains][0] = j * N + i;
      }

      // Leaving a col chain
      if (in_col_chain) {
        // Broken because the current block is not in the chain
        if (game->board[j * N + i] != opposite_player) {
          in_col_chain = 0;
          col_chains[num_col_chains][1] = j * N + i - N;
          ++num_col_chains;
        }

        // Broken because we're at the end of the col
        if (i == N - 1) {
          in_col_chain = 0;
          col_chains[num_col_chains][1] = i * N + j;
          ++num_col_chains;
        }
      }
    }
  }

  // For each chain, if one endpoint is yours, and the other one is empty,
  // the empty one should be set to possible.
  for (int i = 0; i < num_row_chains; ++i) {
    if ((row_chains[i][0] % N > 0) && ((row_chains[i][1] % N) < N - 1)) {
      if (game->board[row_chains[i][0] - 1] == game->player && game->board[row_chains[i][1] + 1] == EMPTY) {
        game->board[row_chains[i][1] + 1] = POSSIBLE;
        game->row_endpoints[0 * (N * N) + row_chains[i][1] + 1] = row_chains[i][0] - 1;
      } else if (game->board[row_chains[i][0] - 1] == EMPTY && game->board[row_chains[i][1] + 1] == game->player) {
        game->board[row_chains[i][0] - 1] = POSSIBLE;
        game->row_endpoints[1 * (N * N) + row_chains[i][0] - 1] = row_chains[i][1] + 1;
      }
    }

    if ((col_chains[i][0] / N > 0) && ((col_chains[i][1] / N) < N - 1)) {
      if (game->board[col_chains[i][0] - N] == game->player && game->board[col_chains[i][1] + N] == EMPTY) {
        game->board[col_chains[i][1] + N] = POSSIBLE;
        game->col_endpoints[0 * (N * N) + col_chains[i][1] + N] = col_chains[i][0] - N;
      } else if (game->board[col_chains[i][0] - N] == EMPTY && game->board[col_chains[i][1] + N] == game->player) {
        game->board[col_chains[i][0] - N] = POSSIBLE;
        game->col_endpoints[1 * (N * N) + col_chains[i][0] - N] = col_chains[i][1] + N;
      }
    }
  }
}

void flip_adjacent(game_t *game, int row, int column)
{
  if (game->row_endpoints[0 * (N * N) + row * N + column] > -1) {
    for (int i = game->row_endpoints[0 * (N * N) + row * N + column] + 1; i < row * N + column; ++i) {
      if (game->board[i] == BLACK) {
        game->board[i] = WHITE;
      } else if (game->board[i] == WHITE) {
        game->board[i] = BLACK;
      }
    }
  }

  if (game->row_endpoints[1 * (N * N) + row * N + column] > -1) {
    for (int i = row * N + column + 1; i < game->row_endpoints[1 * (N * N) + row * N + column]; ++i) {
      if (game->board[i] == BLACK) {
        game->board[i] = WHITE;
      } else if (game->board[i] == WHITE) {
        game->board[i] = BLACK;
      }
    }
  }

  if (game->col_endpoints[0 * (N * N) + row * N + column] > -1) {
    for (int i = game->col_endpoints[0 * (N * N) + row * N + column] + 1; i < row * N + column; i += N) {
      if (game->board[i] == BLACK) {
        game->board[i] = WHITE;
      } else if (game->board[i] == WHITE) {
        game->board[i] = BLACK;
      }
    }
  }

  if (game->col_endpoints[1 * (N * N) + row * N + column] > -1) {
    for (int i = row * N + column + 1; i < game->col_endpoints[1 * (N * N) + row * N + column]; i += N) {
      if (game->board[i] == BLACK) {
        game->board[i] = WHITE;
      } else if (game->board[i] == WHITE) {
        game->board[i] = BLACK;
      }
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
