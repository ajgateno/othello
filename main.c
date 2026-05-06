#include <stdio.h>

#include <SDL.h>

#include "game.h"
#include "logic.h"
#include "rendering.h"

int main(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("Othello", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  game_t game = {
    .board = { 
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, WHITE, BLACK, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, BLACK, WHITE, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
              EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,  
            },
    .player = BLACK,
    .state = STATE_RUNNING
  };

  set_possible_moves(&game);

  SDL_Event e;
  while (game.state == STATE_RUNNING) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          game.state = STATE_QUIT;
          break;

        case SDL_MOUSEBUTTONDOWN:
          click_on_cell(&game, e.button.y / CELL_HEIGHT, e.button.x / CELL_WIDTH);
          printf("Clicked on cell\n");
          set_possible_moves(&game);
          printf("Set the possible moves\n");
          break;

        default: {}
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      render_game(renderer, &game);
      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
