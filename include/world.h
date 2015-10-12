#ifndef SANDY_WORLD_H
#define SANDY_WORLD_H

#include "common.h"

enum pixel_type {
  pix_air,
  pix_sand,
  pix_rock,
  pix_bounds,
  num_pixel_types,
};

enum pixel_flags {
  STATIC    = 1 << 0,
  PASS      = 1 << 1,
  INVISIBLE = 1 << 2,
};

struct pixel_def {
  char *name;
  u8 r, g, b;
  u32 flags;
};

struct pixel {
  float x, y;
  float lx, ly;
  float accel;
  u8 type;
};

extern struct pixel_def pixel_defs[num_pixel_types];
extern struct pixel blank_pixel;

struct world {
  struct pixel* pixels;
  struct pixel* pixels_next;
  struct pixel* current_pixels;
  int last_time, last_dt;
  int num_pixels;
  int w;
  int h;
};

struct pixel*
world_get_pixel_ind (struct world *world, int code, int x, int y);

struct pixel *
init_pixel_at (struct world *world, int x, int y, u8 type, float accel);

struct pixel*
world_get_pixel (struct world *world, int x, int y);

struct world *
world_init(struct world *world, int w, int h);

void world_describe_rle(struct world *world,
                        struct pixel *pixels, int ncols);

void world_clear (struct world *world);
void world_randomize (struct world *world);
void world_swap_buffers(struct world *world);
void world_update(struct world *world);
void world_update_automata(struct world *world);
void world_free(struct world *world);

#endif /* SANDY_WORLD_H */
