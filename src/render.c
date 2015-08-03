
#include "render.h"
#include "world.h"

static inline void
put_pixel(SDL_Surface *surface, int x, int y, struct pixel_def *pixdef) {
  u8 *pixel = (u8*)surface->pixels;
  pixel += (y * surface->pitch) + (x * sizeof(u32));
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    pixel[0] = pixdef->r;
    pixel[1] = pixdef->g;
    pixel[2] = pixdef->b;
  #else
    pixel[0] = pixdef->b;
    pixel[1] = pixdef->g;
    pixel[2] = pixdef->r;
  #endif
}

void render(SDL_Surface *surface, struct world *world) {
  u32 *surface_pixels;
  struct pixel_def *pixdef;
  u8 pixel;

  int i, x, y;

  if(surface->format->BitsPerPixel!=32){
    fprintf(stderr, "Not a 32-bit surface.\n");
    return;
  }

  /* fprintf(stderr, "window surface w %d h %d pitch %d\n", */
  /*         surface->w, */
  /*         surface->h, */
  /*         surface->pitch); */


  surface_pixels = (u32 *)surface->pixels;

  /* SDL_LockSurface(surface); */

  for (x = 0; x < world->w; ++x)
  for (y = 0; y < world->h; ++y) {
    i = y * world->w + x;
    if (i >= world->num_pixels) goto end;
    pixel = world->pixels[i].type;
    pixdef = &pixel_defs[pixel];

    // TODO: precompute this? probably negligible
    put_pixel(surface, x, y, pixdef);
  }

end:

  return;
  /* SDL_UnlockSurface(surface); */
}
