
#include "collide.h"
#include "shapes.h"
#include <stdio.h>

struct collision_data {
  struct world *world;
  struct collision *collision;
  int px, py;
  bool first;
};

static inline bool should_collide(enum pixel_type pixel);

int collide_pixel(void *data, int x, int y) {
  struct collision_data *cd = (struct collision_data*) data;
  struct world *world = cd->world;
  struct collision *c = cd->collision;
  struct pixel *pixel = world_get_pixel_once(world, world->pixels, x, y);

  if (cd->first) {
    cd->first = false;
    return CONTINUE_LINE;
  }

  if (!pixel) return STOP_LINE;

  if(should_collide(pixel->type)) {
    c->x = x;
    c->y = y;
    c->px = cd->px;
    c->py = cd->py;
    c->against = pixel;
    c->collided = true;

    return STOP_LINE;
  }

  cd->px = x;
  cd->py = y;

  return CONTINUE_LINE;
}

struct collision
collide(struct world *world,
        u32 x0, u32 y0, u32 x1, u32 y1,
        u32 fromInd, u32 toInd,
        struct pixel *from, struct pixel *to)
{
  struct collision collision = {
    .collided = false,
    .px = x0,
    .py = y0
  };

  struct collision_data cdata = {
    .world = world,
    .collision = &collision,
    .px = x0,
    .first = true,
    .py = y0
  };

  line((void*)&cdata, x0, y0, x1, y1, collide_pixel);
  return collision;
}


static inline bool
should_collide(enum pixel_type pixel) {
  return (pixel_defs[pixel].flags & PASS) == 0;
}
