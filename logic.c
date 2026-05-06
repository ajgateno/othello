#include <stdio.h>

#include "game.h"
#include "logic.h"

int row_chains[2][N] = { 0 };

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

  int opposite_player;
  if (game->player == BLACK) {
    opposite_player = WHITE;
  } else if (game->player == WHITE) {
    opposite_player = BLACK;
  }

  // for each row, figure out all intervals of consecutive opposite pieces
  int num_row_chains = 0;
  int in_row_chain = 0;

  printf("\n");
  printf("player: ");
  switch(game->player) {
    case BLACK:
      printf("BLACK\n");
      break;

    case WHITE:
      printf("WHITE\n");
      break;
  }

  printf("opposite player: ");
  switch(opposite_player) {
    case BLACK:
      printf("BLACK\n");
      break;

    case WHITE:
      printf("WHITE\n");
      break;
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      // Entering a row chain
      if (!in_row_chain && game->board[i * N + j] == opposite_player) {
        in_row_chain = 1;
        row_chains[num_row_chains][0] = i * N + j;
        printf("Started at (%d, %d)\n", i, j);
      }

      // Leaving a row chain
      if (in_row_chain) {
        // Broken because the current block is not in the chain
        if (game->board[i * N + j] != opposite_player) {
          in_row_chain = 0;
          row_chains[num_row_chains][1] = i * N + j - 1;
          printf("Ended at (%d, %d) because the chain was broken\n", i, j);
          ++num_row_chains;
        }

        // Broken because we're at the end of the row
        if (j == N - 1) {
          in_row_chain = 0;
          row_chains[num_row_chains][1] = i * N + j;
          printf("Ended at (%d, %d) because the row ended\n", i, j);
          ++num_row_chains;
        }
      }
    }
  }

  // For each chain, if one endpoint is yours, and the other one is empty,
  // the empty one should be set to possible.
  printf("There are %d row chains\n", num_row_chains);
  for (int i = 0; i < num_row_chains; ++i) {
    printf("Row chain from %d to %d\n", row_chains[i][0], row_chains[i][1]);
    if ((row_chains[i][0] % N > 0) && ((row_chains[i][1] % N) < N - 1)) {
      printf("GOT HERE A (%d)\n", i);
      if (game->board[row_chains[i][0] - 1] == game->player && game->board[row_chains[i][1] + 1] == EMPTY) {
        game->board[row_chains[i][1] + 1] = POSSIBLE;
      }
      printf("GOT HERE B (%d)\n", i);
      if (game->board[row_chains[i][0] - 1] == EMPTY && game->board[row_chains[i][1] + 1] == game->player) {
        game->board[row_chains[i][0] - 1] = POSSIBLE;
      }
      printf("GOT HERE C (%d)\n", i);
    }
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      switch(game->board[i * N + j]) {
        case BLACK:
          printf("BLACK ");
          break;

        case WHITE:
          printf("WHITE ");
          break;

        case POSSIBLE:
          printf("POSSI ");
          break;

        default: 
          printf("EMPTY ");
      }
    }
    printf("\n");
  }
  printf("\n");
}

void player_turn(game_t *game, int row, int column)
{
  if (game->board[row * N + column] == POSSIBLE) {
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
