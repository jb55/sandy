
#ifndef SANDY_EVENT_H
#define SANDY_EVENT_H

#include "graphics.h"
#include "world.h"

void process_events(const SDL_Renderer *renderer,
                    const SDL_Window *window,
                    struct world *world,
                    SDL_Surface **surface);

#endif /* SANDY_EVENT_H */
