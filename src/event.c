
#include "event.h"
#include "common.h"
#include "world.h"
#include "graphics.h"

void process_events(const SDL_Renderer *renderer,
                    const SDL_Window *window,
                    struct world *world,
                    SDL_Surface **surface) {
  SDL_Event event;
  u16 x, y;
  bool down = false;
  const float def_accel = 0.005f;
  static u8 draw_pixel = pix_rock;
  static bool left_down = false;
  static bool right_down = false;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_WINDOWEVENT_RESIZED:
      /* SDL_SetWindowSize(window, event.window.data1, event.window.data2); */
      break;
    case SDL_MOUSEMOTION:
      x = event.button.x;
      y = event.button.y;
      down = left_down || right_down;

      if (down) {
        init_pixel_at(world, x, y, draw_pixel, def_accel);
        init_pixel_at(world, x+1, y, draw_pixel, def_accel);
        init_pixel_at(world, x-1, y, draw_pixel, def_accel);
        init_pixel_at(world, x, y-1, draw_pixel, def_accel);
        init_pixel_at(world, x, y+1, draw_pixel, def_accel);
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      x = event.button.x;
      y = event.button.y;
      init_pixel_at(world, x, y, draw_pixel, def_accel);

      switch (event.button.button) {
      case SDL_BUTTON_LEFT: left_down = true; break;
      case SDL_BUTTON_RIGHT: right_down = true; break;
      }
      break;
    case SDL_MOUSEBUTTONUP:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT: left_down = false; break;
      case SDL_BUTTON_RIGHT: right_down = false; break;
      }
      break;
    case SDL_KEYUP:
      /* SDL_SetWindowSize(window, event.window.data1, event.window.data2); */
      printf("key %d\n", event.key.keysym.sym);
      char key = event.key.keysym.sym;
      switch (key) {
      case 'r': world_randomize(world); break;
      case 'c':
        // clear the world
        world_clear(world);
        break;
      default:
        if (key >= '0' && key <= '9')
          draw_pixel = key - '0';
        break;
      }
      break;
    case SDL_QUIT:
      SDL_Quit();
      exit(0);
    }
  }
}
