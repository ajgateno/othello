#include <stdio.h>

#include <SDL.h>

#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0

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

  int is_running = 1;

  SDL_Event e;
  while (is_running) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          printf("Quitting!\n");
          is_running = 0;
          break;

        case SDL_MOUSEBUTTONDOWN:
          printf("Click!\n");
          break;

        default: {}
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      SDL_RenderPresent(renderer);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
