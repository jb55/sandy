
#include "event.h"
#include "common.h"
#include "world.h"
#include "graphics.h"
#include "shapes.h"

static const float def_accel = 0.001f;

struct draw_mode {
  struct world *world;
  enum pixel_type type;
};

int world_draw_pixel(void *data, int x, int y) {
  struct draw_mode *dm = (struct draw_mode*)data;
  struct world *world = dm->world;
  init_pixel_at(world, x, y, dm->type, def_accel);
  return 0;
}

void process_events(const SDL_Renderer *renderer,
                    const SDL_Window *window,
                    struct world *world,
                    SDL_Surface **surface) {
  SDL_Event event;
  u16 x, y;
  static u32 draw_x, draw_y;
  bool down = false;
  static bool drawing = false;
  static u8 draw_pixel = pix_rock;
  static bool left_down = false;
  static bool right_down = false;
  static bool draw_mode = false;
  struct draw_mode draw_data;
  draw_data.world = world;

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
        if (draw_mode) {
          if (!drawing) {
            drawing = true;
            draw_x = x;
            draw_y = y;
          }
        }
        else {
          init_pixel_at(world, x, y, draw_pixel, def_accel);
          init_pixel_at(world, x+1, y, draw_pixel, def_accel);
          init_pixel_at(world, x-1, y, draw_pixel, def_accel);
          init_pixel_at(world, x, y-1, draw_pixel, def_accel);
          init_pixel_at(world, x, y+1, draw_pixel, def_accel);
        }
      } else {
        if (drawing && draw_mode) {
          draw_data.type = draw_pixel;
          line((void*)&draw_data, draw_x, draw_y, x, y, world_draw_pixel);
          drawing = false;
        }
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
      case 'd':
        draw_mode = !draw_mode;
        if (draw_mode) printf("draw mode on\n");
        else printf("draw mode off\n");
        break;
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
