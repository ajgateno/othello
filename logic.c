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

int check_direction(game_t *game, int row, int column, int row_increment, int column_increment)
{
  int opposite_player;
  if (game->player == BLACK) {
    opposite_player = WHITE;
  } else if (game->player == WHITE) {
    opposite_player = BLACK;
  }

  int i;

  if (row + row_increment < 0 || row + row_increment >= N || column + column_increment < 0 || column + column_increment >= N) {
    return -1;
  }

  for (i = 1; i < N; ++i) {
    if (game->board[(row + i * row_increment) * N + (column + i * column_increment)] != opposite_player) {
      break;
    }
  }

  if (i > 1 && game->board[(row + i * row_increment) * N + (column + i * column_increment)] == game->player) {
    return (row + i * row_increment) * N + (column + i * column_increment);
  }

  return -1;
}

void set_possible_move(game_t *game, int row, int column)
{
  if (game->board[row * N + column] != EMPTY) {
    return;
  }

  game->possible_moves[row * N + column].left = check_direction(game, row, column, 0, -1);
  game->possible_moves[row * N + column].right = check_direction(game, row, column, 0, 1);
  game->possible_moves[row * N + column].up = check_direction(game, row, column, -1, 0);
  game->possible_moves[row * N + column].down = check_direction(game, row, column, 1, 0);
  game->possible_moves[row * N + column].ne = check_direction(game, row, column, -1, -1);
  game->possible_moves[row * N + column].se = check_direction(game, row, column, 1, -1);
  game->possible_moves[row * N + column].nw = check_direction(game, row, column, -1, 1);
  game->possible_moves[row * N + column].sw = check_direction(game, row, column, 1, 1);
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

    game->possible_moves[i].nw = -1;
    game->possible_moves[i].ne = -1;
    game->possible_moves[i].sw = -1;
    game->possible_moves[i].se = -1;
  }

  if (game->state != STATE_RUNNING) {
    return;
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

      if (game->possible_moves[i * N + j].nw > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].ne > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].sw > -1) {
        game->board[i * N + j] = POSSIBLE;
      }

      if (game->possible_moves[i * N + j].se > -1) {
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

void flip_adjacent_direction(game_t *game, int row, int column, int row_increment, int column_increment, int endpoint)
{
  if (endpoint > -1) {
    for (int i = 1; (row + i * row_increment) * N + (column + i * column_increment) != endpoint; ++i) {
      flip_cell(game, row + i * row_increment, column + i * column_increment);
    }
  }
}

void flip_adjacent(game_t *game, int row, int column)
{
  flip_adjacent_direction(game, row, column, 0, -1, game->possible_moves[row * N + column].left);
  flip_adjacent_direction(game, row, column, 0, 1, game->possible_moves[row * N + column].right);
  flip_adjacent_direction(game, row, column, -1, 0, game->possible_moves[row * N + column].up);
  flip_adjacent_direction(game, row, column, 1, 0, game->possible_moves[row * N + column].down);
  flip_adjacent_direction(game, row, column, -1, -1, game->possible_moves[row * N + column].ne);
  flip_adjacent_direction(game, row, column, 1, -1, game->possible_moves[row * N + column].se);
  flip_adjacent_direction(game, row, column, -1, 1, game->possible_moves[row * N + column].nw);
  flip_adjacent_direction(game, row, column, 1, 1, game->possible_moves[row * N + column].sw);
}

int count_pieces(game_t *game, int piece_type) 
{
  int count = 0;

  for (int i = 0; i < N * N; ++i) {
    count += (game->board[i] == piece_type);
  }

  return count;
}

void check_game_over(game_t *game) 
{
  set_possible_moves(game);

  if (count_pieces(game, POSSIBLE) == 0) {
    int black_score = count_pieces(game, BLACK);
    int white_score = count_pieces(game, WHITE);

    if (black_score > white_score) {
      game->state = STATE_BLACK_WON;
    } else if (white_score > black_score) {
      game->state = STATE_WHITE_WON;
    } else {
      game->state = STATE_DRAW;
    }
  }

  set_possible_moves(game);
}

void player_turn(game_t *game, int row, int column)
{
  if (game->board[row * N + column] == POSSIBLE) {
    game->board[row * N + column] = game->player;
    flip_adjacent(game, row, column);
    switch_player(game);
    check_game_over(game);
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
