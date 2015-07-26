
#include "render.h"

void render(SDL_Surface *surface, struct world *world) {
  SDL_PixelFormat fmt;
  Uint32 surface_size;
  int i = 0;

  if(surface->format->BitsPerPixel!=32){
    fprintf(stderr, "Not a 32-bit surface.\n");
    return;
  }

  SDL_LockSurface(surface);

  surface_size = surface->w * surface->h;
  for (i = 0; i < surface_size; i++) {
    ((Uint32 *)surface->pixels)[i] =
      ((int)floor(sin(i)*200)) << 16 |
      (i % 0xFF);
  }

  SDL_UnlockSurface(surface);
}
