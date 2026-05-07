#ifndef GAME_H_
#define GAME_H_

#define N 8
#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0
#define CELL_WIDTH (SCREEN_WIDTH / N)
#define CELL_HEIGHT (SCREEN_HEIGHT / N)

#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define POSSIBLE 3

#define STATE_RUNNING 0
#define STATE_BLACK_WON 1
#define STATE_WHITE_WON 2
#define STATE_DRAW 3
#define STATE_QUIT 4

// A -1 determines no possible move in that direction
// A non-negative integer determines the position that bookends a move
typedef struct {
  int up;
  int down;
  int left;
  int right;
} possible_move_t;

typedef struct {
  int board[N * N];
  possible_move_t possible_moves[N * N];

  int player;
  int state;

  int row_buffer[N];
  int col_buffer[N];
} game_t;

#endif // GAME_H_
