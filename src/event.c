
#include "event.h"
#include "graphics.h"

void process_events(const SDL_Renderer *renderer,
                    const SDL_Window *window,
                    SDL_Surface **surface) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_WINDOWEVENT_RESIZED:
      SDL_SetWindowSize(window, event.window.data1, event.window.data2);
      break;
    case SDL_QUIT:
      SDL_Quit();
      exit(0);
    }
  }
}
