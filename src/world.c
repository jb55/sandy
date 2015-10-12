
#include "world.h"
#include "morton.h"
#include "common.h"
#include "graphics.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct pixel blank_pixel = {
  .x = 0.0f,
  .y = 0.0f,
  .lx = 0.0f,
  .ly = 0.0f,
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
  return (pixel_defs[pixel].flags & PASS) == 0;
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

static inline int
move_pixel(struct world *world, struct pixel *pixel, int to) {
  if (to > world->num_pixels) return 0;
  world->pixels_next[to] = *pixel;
  return 1;
}

struct pixel *
world_get_pixel_ind (struct world *world, int code, int x, int y) {
  if (y < 0 || y >= world->h) return NULL;
  if (x < 0 || x >= world->w) return NULL;
  if (code >= world->num_pixels) return NULL;
  return &world->pixels[code];
}

struct pixel *
init_pixel_at (struct world *world, int x, int y, u8 type, float accel) {
  if (y < 0 || y >= world->h) return NULL;
  if (x < 0 || x >= world->w) return NULL;
  struct pixel *pixel = &world->pixels[world_index_pixels(world, x, y)];

  pixel->accel = accel;
  pixel->type = type;
  pixel->x = x;
  pixel->y = y;
  pixel->lx = x;
  pixel->ly = y;

  return pixel;
}

struct pixel *
world_get_pixel (struct world *world, int x, int y) {
  if (y < 0 || y >= world->h) return NULL;
  if (x < 0 || x >= world->w) return NULL;
  return &world->pixels[world_index_pixels(world, x, y)];
}

static inline struct pixel *
world_get_pixel_once (struct world *world, struct pixel *pixels, int x, int y) {
  if (y < 0 || y >= world->h) return NULL;
  if (x < 0 || x >= world->w) return NULL;
  return &pixels[world_index_pixels(world, x, y)];
}

void
world_randomize (struct world *world) {
  int x = 0, y = 0;
  u8 pixel_type = pix_air;
  struct pixel *pixel;
  for (x = 0; x < world->w; ++x)
  for (y = 0; y < world->h; ++y) {
    pixel_type = rand() % 16;
    pixel_type = pixel_type > 4 || pixel_type == pix_bounds ? pix_air : pixel_type;

    pixel = world_get_pixel_once(world, world->pixels, x, y);

    if (!pixel) continue;

    pixel->accel = 0.001f;
    pixel->type = pixel_type;
    pixel->x = pixel->lx = (float)x;
    pixel->y = pixel->ly = (float)y;
  }
}

void
show_pixel(struct pixel *pixel) {
  printf("pixel (x %f y %f lx %f ly %f accel %f)",
         pixel->x, pixel->y, pixel->lx, pixel->ly, pixel->accel);
}

void
world_update_automata(struct world *world) {
  int x = 0, y = 0;
  int dt;
  float px = 0, py = 0, dx = 0, dy = 0, rdy = 0;
  float dt_ratio;
  int ix = 0, iy = 0;
  int start_time, end_time;
  bool first = true;
  int num_pixels;

  num_pixels = world->w * world->h;
  start_time = SDL_GetTicks();

  // verlet stuff
  dt = world->last_time == 0 ? start_time : start_time - world->last_time;
  dt_ratio = world->last_dt == 0 ? 1 : ((float)dt) / ((float)world->last_dt);

  for (y = 0; y < world->h; y++)
  for (x = 0; x < world->w; x++) {

    u32 middlei = world_index_pixels(world, x, y);
    struct pixel *middle = world_get_pixel_ind(world, middlei, x, y);

    if (!should_move(middle->type)) {
      move_pixel(world, middle, middlei);
      continue;
    }

    rdy = middle->y - middle->ly;

    px = middle->x; /* + (middle->x - middle->lx) * dt_ratio + middle->accel * dt * dt; */
    py = middle->y + rdy * dt_ratio + middle->accel * dt * dt;

    middle->lx = middle->x;
    middle->ly = middle->y;

    dx = fabs(px - middle->x);
    dy = fabs(py - middle->y);

    ix = (int)px;
    iy = (int)py;

    if (middle->type != 0 && first) {
      show_pixel(middle);
      printf("rdy %f px %f py %f dx %f dy %f ix %d iy %d \n", rdy, px, py, dx, dy, ix, iy);
      first = false;
    }

    middle->x = px;
    middle->y = py;

    u32 new_posi = world_index_pixels(world, ix, iy);
    struct pixel *new_pos = world_get_pixel_ind(world, new_posi, ix, iy);

    if (!new_pos) {
      move_pixel(world, middle, middlei);
      continue;
    }

    if (new_posi != middlei) {
      move_pixel(world, middle, new_posi);
      /* move_pixel(world, middle, middle); */
      /* clear_pixel(world, middlei); */
      /* show_pixel(new_pos); */
    } else {
      move_pixel(world, middle, middlei);
    }
  }

  world->last_dt = dt;
  world->last_time = start_time;


  end_time = SDL_GetTicks();
}

void world_describe_rle(struct world *world,
                        struct pixel *pixels, int ncols) {
  int x = 0, y = 0;
  int count = 1;
  int cols = 0;
  int last_type = pix_air;
  struct pixel *pixel;

  for (y = 0; y < world->h; ++y)
  for (x = 0; x < world->w; ++x) {
    pixel = world_get_pixel_once(world, pixels, x, y);
    if (pixel->type == last_type) {
      count++;
      continue;
    }
    else {
      printf("%d*%d\t", last_type, count);
      if (++cols > ncols) printf("\n");
      count = 1;
      cols = 0;
    }
    last_type = pixel->type;
  }

  printf("%d*%d\t", last_type, count);
  printf("\n");
}

void
world_swap_buffers(struct world *world) {
  struct pixel *pixels_tmp = world->pixels;
  world->pixels = world->pixels_next;
  world->pixels_next = pixels_tmp;
  memset(world->pixels_next, pix_air, world->w * world->h * sizeof(struct pixel));
}

void
world_free(struct world *world) {
  free(world->pixels);
  free(world->pixels_next);
}

