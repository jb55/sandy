
#include "graphics.h"
#include "event.h"
#include "render.h"

#include <assert.h>
#include <time.h>
#include <stdlib.h>


int main(void) {
  SDL_Surface *surface;
  int start_time, end_time;
  struct world world;

  srand(time(NULL));

  world_init(&world, 640, 480);
  world_randomize(&world);

  SDL_Window *window = SDL_CreateWindow(
    "Sandy", 0, 0, 640, 480, SDL_WINDOW_RESIZABLE);

  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  while (1) {
    process_events(renderer, window, &world, &surface);

    surface = SDL_GetWindowSurface(window);

    start_time = SDL_GetTicks();

    world_update(&world);
    render(surface, &world);

    end_time = SDL_GetTicks();
    printf("update + render %d ms\n", end_time - start_time);

    SDL_RenderPresent(renderer);

    world_swap_buffers(&world);

    SDL_Delay(6);
    SDL_RenderClear(renderer);
    /* SDL_LoadObject("src/libsandy.so"); */
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

