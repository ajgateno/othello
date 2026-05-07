#ifndef LOGIC_H_
#define LOGIC_H_

#include "game.h"

void set_possible_moves(game_t *game);

void click_on_cell(game_t *game, int row, int col);

void buffer_row(game_t *game, int row);
void buffer_col(game_t *game, int col);

#endif // LOGIC_H_
