
#ifndef SANDY_COLLISION_H
#define SANDY_COLLISION_H

#include "common.h"
#include "world.h"

struct collision {
  bool collided;
  struct pixel *against;
  u32 x, y, px, py;
};

struct collision
collide(struct world *world,
        u32 x0, u32 y0, u32 x1, u32 y1,
        u32 fromInd, u32 toInd,
        struct pixel *from, struct pixel *to);

#endif /* SANDY_COLLISION_H */
