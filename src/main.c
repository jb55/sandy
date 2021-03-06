
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
  /* world_randomize(&world); */

  SDL_Window *window = SDL_CreateWindow(
    "Sandy", 0, 0, 640, 480, SDL_WINDOW_RESIZABLE);

  SDL_Renderer *renderer =
    SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

  while (1) {
    process_events(renderer, window, &world, &surface);

    surface = SDL_GetWindowSurface(window);

    start_time = SDL_GetTicks();

    world_update_automata(&world);
    render(surface, &world);

    end_time = SDL_GetTicks();
    /* printf("update + render %d ms\n", end_time - start_time); */

    SDL_RenderPresent(renderer);

    /* printf("pcur "); world_describe_rle(&world, world.pixels, 10); */
    /* printf("pnxt "); world_describe_rle(&world, world.pixels_next, 10); */

    world_swap_buffers(&world);

    /* printf("pcur %p pnxt %p\n", world.pixels, world.pixels_next); */

    /* printf("ncur "); world_describe_rle(&world, world.pixels, 10); */
    /* printf("nnxt "); world_describe_rle(&world, world.pixels_next, 10); */

    SDL_Delay(6);
    SDL_RenderClear(renderer);
    /* SDL_LoadObject("src/libsandy.so"); */
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

