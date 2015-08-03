
#include "world.h"
#include "morton.h"
#include "common.h"
#include "graphics.h"

#include <stdlib.h>
#include <stdio.h>

struct pixel blank_pixel = {
  .x = 0.0f,
  .y = 0.0f,
  .accel = 0.0f,
  .type = pix_air
};

struct pixel_def pixel_defs[] = {
  { .name = "air", .flags = INVISIBLE | PASS | STATIC },
  /* { .name = "sand", .r = 255, .g = 0, .b = 0, .flags = 0 }, */
  { .name = "sand", .r = 229, .g = 218, .b = 186, .flags = 0 },
  { .name = "rock", .r = 100, .g = 100, .b = 100, .flags = STATIC },
  { .name = "bounds", .r = 255, .g = 0, .b = 0, .flags = INVISIBLE | STATIC },
};

struct world *
world_init(struct world *world, int w, int h) {
  size_t size = w * h;
  world->pixels = (struct pixel*)malloc(size * sizeof(struct pixel));
  world->pixels_next = (struct pixel*)malloc(size * sizeof(struct pixel));
  world->w = w;
  world->h = h;
  world->num_pixels = size;
  return world;
}

void
world_clear (struct world *world) {
  memset(world->pixels, pix_air, world->num_pixels * sizeof(struct pixel));
}

static inline u32
world_index_pixels (struct world *world, int x, int y) {
  /* return morton_encode2(x, y); */
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

static inline void
clear_pixel(struct world *world, int to) {
  if (to > world->num_pixels) return;
  world->pixels_next[to] = blank_pixel;
}

static inline void
move_pixel(struct world *world, struct pixel *pixel, int to) {
  if (to > world->num_pixels) return;
  world->pixels_next[to] = *pixel;
}

static inline struct pixel *
world_get_pixel (struct world *world, int code, int x, int y) {
  if (y < 0 || y >= world->h) return NULL;
  if (x < 0 || x >= world->w) return NULL;
  if (code >= world->num_pixels) return NULL;
  return &world->pixels[code];
}

void
world_set_pixel_type (struct world *world, int x, int y, u8 type) {
  if (y < 0 || y >= world->h) return;
  if (x < 0 || x >= world->w) return;
  world->pixels[world_index_pixels(world, x, y)].type = type;
}

void
world_randomize (struct world *world) {
  int x = 0, y = 0;
  u8 pixel = pix_air;
  struct pixel *pixels = world->pixels;
  for (x = 0; x < world->w; ++x)
  for (y = 0; y < world->h; ++y) {
    pixel = rand() % 16;
    pixel = pixel > 4 || pixel == pix_bounds ? pix_air : pixel;

    pixels[world_index_pixels(world, x, y)].type = pixel;
  }
}

void
world_update(struct world *world) {
  int x = 0, y = 0;
  int ix = 0, iy = 0;
  int dt;
  float px = 0, py = 0;
  float dt_ratio;
  int start_time, end_time;
  int num_pixels;
  /* u8 *pixels = world->pixels; */
  struct pixel *pixels_next = world->pixels_next;

  num_pixels = world->w * world->h;
  start_time = SDL_GetTicks();

  // verlet stuff
  dt = start_time - world->last_time;
  dt_ratio = dt / (world->last_dt == 0 ? 1 : world->last_dt);

  for (y = world->h-1; y >= 0; y--)
  for (x = world->w-1; x >= 0; x--) {
    u32 middlei = world_index_pixels(world, x, y);
    struct pixel *middle = world_get_pixel(world, middlei, x, y);

    u8 middlet = middle->type;
    /* u32 topi    = morton_encode2(x, y-1); */
    /* u32 righti  = morton_encode2(x+1, y); */
    /* u32 lefti   = morton_encode2(x-1, y); */

    /* u8 top    = get_pixel_type(world, topi, x, y-1); */
    /* u8 right  = get_pixel_type(world, righti, x+1, y); */
    /* u8 left   = get_pixel_type(world, lefti, x-1, y); */

    if (should_move(middle->type)) {
      middle->accel = 0.01f;
      px = middle->x + (x - middle->lx) * dt_ratio + middle->accel * dt * dt * 0.001f;
      py = middle->y + (x - middle->ly) * dt_ratio + middle->accel * dt * dt * 0.001f;

      middle->lx = px;
      middle->ly = py;

      ix = (int)px;
      iy = (int)py;

      u32 new_posi = world_index_pixels(world, ix, iy+1);
      struct pixel *new_pos = world_get_pixel(world, new_posi, x, y+1);

      if (new_pos && should_collide(new_pos->type)) {
        move_pixel(world, middle, middlei);
        clear_pixel(world, middlei);
      }
      else {
        move_pixel(world, middle, middlei);
      }
    }
    else {
      pixels_next[middlei].type = middlet;
    }
  }

  world->last_dt = dt;
  world->last_time = start_time;

  end_time = SDL_GetTicks();

  /* printf("world update %d ms\n", end_time - start_time); */
}

void
world_swap_buffers(struct world *world) {
  struct pixel *pixels_tmp = world->pixels;
  world->pixels = world->pixels_next;
  world->pixels_next = pixels_tmp;
  memset(world->pixels_next, pix_air, world->w * world->h);
}

void
world_free(struct world *world) {
  free(world->pixels);
  free(world->pixels_next);
}

