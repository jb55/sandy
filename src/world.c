
#include "world.h"
#include "morton.h"
#include "common.h"
#include "graphics.h"

#include <stdlib.h>
#include <stdio.h>

struct pixel pixel_defs[] = {
  { .name = "air", .flags = INVISIBLE | PASS | STATIC },
  /* { .name = "sand", .r = 255, .g = 0, .b = 0, .flags = 0 }, */
  { .name = "sand", .r = 229, .g = 218, .b = 186, .flags = 0 },
  { .name = "rock", .r = 100, .g = 100, .b = 100, .flags = STATIC },
  { .name = "bounds", .r = 255, .g = 0, .b = 0, .flags = INVISIBLE | STATIC },
};

struct world *
world_init(struct world *world, int w, int h) {
  world->pixels = (u8*)malloc((size_t)w * h * 2);
  world->pixels_next = (u8*)malloc((size_t)w * h * 2);
  world->w = w;
  world->h = h;
}

static inline u32
index_pixels (struct world *world, int x, int y) {
  return y * world->w + x;
}

static inline bool
should_collide(u8 pixel) {
  return (pixel_defs[pixel].flags & PASS) != 0;
}

static inline bool
should_move(u8 pixel) {
  return (pixel_defs[pixel].flags & STATIC) == 0;
}

static inline u8
get_pixel (struct world *world, u32 code, int x, int y) {
  if (y < 0 || y > world->h) return pix_bounds;
  if (x < 0 || x > world->w) return pix_bounds;
  return world->pixels[code];
}

void
world_set_pixel (struct world *world, int x, int y, u8 pixel) {
  if (y < 0 || y > world->h) return;
  if (x < 0 || x > world->w) return;
  world->pixels[index_pixels(world, x, y)] = pixel;
}

void
world_randomize (struct world *world) {
  int x = 0, y = 0;
  u8 pixel = pix_air;
  u8 *pixels = world->pixels;
  for (x = 0; x < world->w; ++x)
  for (y = 0; y < world->h; ++y) {
    pixel = rand() % 16;
    pixel = pixel > 4 || pixel == pix_bounds ? pix_air : pixel;

    pixels[index_pixels(world, x, y)] = pixel;
  }
}

void
world_update(struct world *world) {
  int x = 0, y = 0;
  int start_time, end_time;
  int num_pixels;
  /* u8 *pixels = world->pixels; */
  u8 *pixels_next = world->pixels_next;

  num_pixels = world->w * world->h;
  start_time = SDL_GetTicks();

  for (y = world->h-1; y >= 0; y--)
  for (x = world->w-1; x >= 0; x--) {
    u32 middlei = index_pixels(world, x, y);
    u8 middle = get_pixel(world, middlei, x, y);
    /* u32 topi    = morton_encode2(x, y-1); */
    /* u32 righti  = morton_encode2(x+1, y); */
    /* u32 lefti   = morton_encode2(x-1, y); */

    /* u8 top    = get_pixel(world, topi, x, y-1); */
    /* u8 right  = get_pixel(world, righti, x+1, y); */
    /* u8 left   = get_pixel(world, lefti, x-1, y); */

    if (should_move(middle)) {
      u32 bottomi = index_pixels(world, x, y+1);
      u8 bottom = get_pixel(world, bottomi, x, y+1);
      if (should_collide(bottom)) {
        pixels_next[middlei] = pix_air;
        pixels_next[bottomi] = middle;
      }
      else {
        pixels_next[middlei] = middle;
      }
    }
    else {
      pixels_next[middlei] = middle;
    }
  }

  end_time = SDL_GetTicks();
  /* printf("world update %d ms\n", end_time - start_time); */
}

void
world_swap_buffers(struct world *world) {
  u8 *pixels_tmp = world->pixels;
  world->pixels = world->pixels_next;
  world->pixels_next = pixels_tmp;
  memset(world->pixels_next, pix_air, world->w * world->h);
}

void
world_free(struct world *world) {
  free(world->pixels);
  free(world->pixels_next);
}

